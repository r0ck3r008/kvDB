#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include"objstore/ptree.h"

/* General utilities */
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

int
hash_it(char a)
{
    return a-'a';
}

/* Pnode related functions */
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

/* Ptree related functions */
Ptree *
ptree_init()
{
    Ptree *ptree = calloc(1, sizeof(Ptree));
    if(ptree == NULL) {
        fprintf(stderr, "[-]PTREE: Malloc: Error in allocating memory\n");
        _exit(1);
    }
    ptree->root = pnode_init(NULL, NULL);

    return ptree;
}

void
ptree_insert(Ptree *ptree, char *key, char *value)
{
    pnode_insert(ptree->root, key, value);
}

Pnode *
ptree_find(Ptree *ptree, char *key)
{
    return pnode_find(ptree->root, key);
}

void
ptree_deinit(Ptree *ptree)
{
    pnode_deinit(ptree->root);
    free(ptree);
}