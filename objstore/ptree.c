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
            len = find_len(pnode->key, key, l1, l2),
            indx1 = hash_it(pnode->key[len]),
            indx2 = hash_it(key[len]);
        if(len == l1 && len == l2) {
            fprintf(stderr, "[-] Cannot Add duplicate keys!\n");
            _exit(1);
        }
        /* Form new children nodes */
        char buf1[512] = {0}, buf2[512] = {0};
        sprintf(buf1, "%s", &(pnode->key[len]));
        sprintf(buf2, "%s", &(key[len]));
        pnode->child[indx1] = pnode_insert(pnode->child[indx1], buf1, pnode->value);
        pnode->child[indx2] = pnode_insert(pnode->child[indx2], buf2, value);
        /* Update current node and cleanup as needed */
        char *old_key = pnode->key;
        if(len)
            pnode->key = str_cpy(pnode->key, len);
        else
            /* Since the keys completely differ, we seprate the two and make
             * current key NULL
             */
            pnode->key = NULL;
        pnode->value = NULL;
        free(pnode->value);
        free(old_key);
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
pnode_find(Pnode *pnode, char *key)
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
            if(l1 == l2)
                /* If both keys are equal, return current node */
                return pnode;
            else
                /* If key is substring of pnode->key, return error.
                 * Also, note that l2 cannot be > l1 since l2 == len.
                 */
                return NULL;

        } else {
            /* Chop the target key and recurse */
            int indx = hash_it(key[len]);
            char buf[512] = {0};
            sprintf(buf, "%s", &(key[len]));
            return pnode_find(pnode->child[indx], buf);
        }

    } else {
        /* It is a node akin to root node, recurse to closest child */
        int indx = hash_it(key[0]);
        return pnode_find(pnode->child[indx], key);
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