#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include"objstore/ptree.h"

/* General utilities */
/*
 * Creates a copy of given string upto given length by allocating a new buffer.
 * @param str: The input string to copy.
 * @param len: The number of characters to copy, -1 indicates whole length.
 * @return The allocated buffer, needs to be freed at later time.
 */
char *
str_cpy(char *str, int len)
{
    if(str==NULL)
        return NULL;
    else if(len==-1)
        len = strlen(str);
    char *ret = strndup(str, len);
    if(str==NULL) {
        fprintf(stderr, "[-]STRCPY: Malloc: Error in allocating memory\n");
        _exit(1);
    }

    return ret;
}

/*
 * Finds the maximum character length upto which the given strings are equal.
 * @param s1: A char pointer to the first string.
 * @param s2: A char pointer to the second string.
 * @param s3: Length of the first string.
 * @param s3: Length of the second string.
 * @return int max length.
 */
int
find_len(char *s1, char *s2, int l1, int l2)
{
    int mlen = ((l1 < l2) ? (l1) : (l2));
    
    int len;
    for(len=0; len<mlen; len++) {
        if(s1[len] != s2[len])
            break;
    }

    return len;
}

/*
 * An extensible function to implement mapping of a character to its position in child array
 * of a node.
 * @param a: A character which needs to be hashed.
 * @return The hashed/mapped int value.
 */
int
hash_it(char a)
{
    return a-'a';
}

/*
 * Chop the given key and push it to the children.
 * @param pnode: Pointer to Pnode whose children need to be recursed on.
 * @param key: Character pointer to the key to be chopped.
 * @param value: The value to pass along.
 * @param len: Point where to chop.
 * @return void.
 */
void
chopPush(Pnode *pnode, char *key, char *value, int len)
{
    int indx = hash_it(key[len]);
    char buf[512] = {0};
    sprintf(buf, "%s", &(key[len]));
    pnode->child[indx] = pnode_insert(pnode->child[indx], buf, value);
}

void
pushToKeyList(Pnode *pnode, KeyList **klist)
{
    *klist = keylist_prepend(*klist, pnode->key, pnode->value);
    for(int i=0; i<26; i++) {
        if(pnode->child[i]!=NULL)
            pushToKeyList(pnode->child[i], klist);
    }
}

/* Pnode related functions */
/*
 * An initializing function for Pnode. Allocates a new node and returns a pointer to it.
 * @param key: A char pointer to the key which will be used to store within the prefix tree.
 * @param value: A char pointer which serves as the payload mapped by key within prefix tree.
 * @return Pointer to a newly allocated Pnode.
 */
Pnode *
pnode_init(char *key, char *value)
{
    Pnode *pnode = calloc(1, sizeof(Pnode));
    if(pnode == NULL) {
        fprintf(stderr, "[-]PNODE: Malloc: Error in allocating memory\n");
        _exit(1);
    }
    pnode->key = str_cpy(key, -1);
    pnode->value = str_cpy(value, -1);

    return pnode;
}

/*
 * Recursively insert into the prefix tree based.
 * @param pnode: Pointer to a Pnode.
 * @param key: Character pointer to a proper subset of key, may be chopped recursively.
 * @param value: The payload that key will eventually map to within the tree.
 * return Pointer to Pnode same as input parameter pnode.
 */
Pnode *
pnode_insert(Pnode *pnode, char *key, char *value)
{
    if(pnode==NULL) {
        pnode = pnode_init(key, value);
    } else if (pnode->key!=NULL) {
        /* Determine chopping positions */
        int l1 = strlen(pnode->key), l2 = strlen(key),
            len = find_len(pnode->key, key, l1, l2);
        if(len == l1 && len == l2) {
            fprintf(stderr, "[-] Cannot Add duplicate keys!\n");
            _exit(1);
        } else if(len == l1) {
            /* Chop and push the new key further down and skip current one */
            chopPush(pnode, key, value, len);
        } else if(len == l2) {
            /* Chop and push the curr key further down skipping new one and save new value here */
            chopPush(pnode, pnode->key, pnode->value, len);
            pnode->value = value;
        } else {
            /* Chop and push both current and new keys */
            /* Form a new node */
            Pnode *new_pnode;
            if(len!=0) {
                /* The first half of current key goes to new node */
                char buf[512] = {0}, buf2[512] = {0};
                snprintf(buf, len+1, "%s", pnode->key);
                new_pnode = pnode_init(buf, NULL);
                /* Second half goes to old node */
                char *old_key = pnode->key;
                sprintf(buf2, "%s", &(pnode->key[len]));
                pnode->key = str_cpy(buf2, -1);
                free(old_key);
            } else {
                /* Since len == 0, node key updation is required between new and old nodes */
                new_pnode = pnode_init(NULL, NULL);
            }
            /* Push new key down the new node */
            chopPush(new_pnode, key, value, len);
            /* Make new node parent of old node */
            int indx = hash_it(pnode->key[0]);
            new_pnode->child[indx] = pnode;

            /* Now return the new node */
            pnode = new_pnode;
        }
    } else {
        int indx = hash_it(key[0]);
        pnode->child[indx] = pnode_insert(pnode->child[indx], key, value);
    }

    return pnode;
}

/*
 * Recursively find if a key mapping exists in the tree.
 * @param pnode: Pointer to Pnode from where to being looking.
 * @param key: Character pointer to the key for which to look for.
 * @return NULL if key is not found or pointer to Pnode where mapped value resides.
 */
Pnode *
pnode_find(Pnode *pnode, char *key, KeyList **klist)
{
    if(pnode==NULL) {
        return NULL;
    } else if(pnode->key!=NULL) {
        int l1 = strlen(pnode->key), l2 = strlen(key),
            len = find_len(pnode->key, key, l1, l2);
        if(len == 0) {
            /* If both keys totally differ, no need to recurse, return */
            return NULL;
        } else if(len == l2) {
            /* Return true only if both keys are totally equal */
            if(l1 == l2) {
                /* If both keys are equal, return current node */
                return pnode;
            } else {
                /* If key is substring of pnode->key, return error.
                 * Also, note that l2 cannot be > l1 since l2 == len.
                 */
                pushToKeyList(pnode, klist);
                return NULL;
            }

        } else {
            /* Chop the target key and recurse */
            int indx = hash_it(key[len]);
            char buf[512] = {0};
            sprintf(buf, "%s", &(key[len]));
            Pnode *find = NULL;
            if((find=pnode_find(pnode->child[indx], buf, klist)) == NULL) {
                pushToKeyList(pnode, klist);
                return NULL;
            } else {
                return find;
            }
        }
    } else if(pnode->value!=NULL) {
        return pnode;
    } else {
        /* It is a node akin to root node, recurse to closest child */
        int indx = hash_it(key[0]);
        return pnode_find(pnode->child[indx], key, klist);
    }
}

/*
 * Destroy and free the space recursively of a given Pnode.
 * @param pnode: Pointer to the Pnode to recurse and eventually destroy.
 * @return void.
 */
void
pnode_deinit(Pnode *pnode)
{
    for(int i=0; i<26; i++) {
        if(pnode->child[i]!=NULL)
            pnode_deinit(pnode->child[i]);
    }
    /* No NULL checks since free can digest NULLs */
    free(pnode->key);
    free(pnode->value);
    free(pnode);
}