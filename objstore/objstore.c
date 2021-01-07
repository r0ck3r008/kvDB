#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include"objstore/objstore.h"
#include "objstore/ptree.h"

/*
 * Allocate and initialize an ObjStore object and return a pointer to it.
 * @param None.
 * @return Pointer to initialized ObjStore object.
 */
ObjStore *
objstore_init()
{
    ObjStore *ost = calloc(1, sizeof(ObjStore));
    if(ost==NULL) {
        fprintf(stderr, "[-]OBJSTORE: Malloc: Error in allocating memory!\n");
        _exit(1);
    }
    ost->tree = pnode_init(NULL, NULL);

    return ost;
}

/*
 * Insert the given key value pair into the Prefix Tree.
 * @param ost: Pointer to the ObjStore mantaining the Prefix Tree.
 * @param key: Character pointer to the key string.
 * @param value: Character pointer to the value string.
 * @return void.
 */
void
objstore_insert(ObjStore *ost, char *key, char *value)
{
    ost->curr = NULL;
    pnode_insert(ost->tree, key, value);
}

/*
 * Find the given key if it exists within the prefix tree beginning
 * either from the stored current Pnode or the root of the tree.
 * @param ost: ObjStore pointer abstracting the Prefix Tree.
 * @param key: Character pointer to the key to look for.
 */
Pnode *
objstore_find(ObjStore *ost, char *key)
{
    Pnode *res = NULL;
    if(ost->curr!=NULL) {
        res = pnode_find(ost->curr, key);
    } else {
        res = pnode_find(ost->tree, key);
    }

    return res;
}

/*
 * Deinitialize and free the given ObjStore pointer.
 * @param ost: Pointer to the ObjStore object to be deinitialized.
 * @return void.
 */
void
objstore_deinit(ObjStore *ost)
{
    pnode_deinit(ost->tree);
    free(ost);
}