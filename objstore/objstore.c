#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include"objstore/objstore.h"
#include "objstore/ptree.h"

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

void
objstore_insert(ObjStore *ost, char *key, char *value)
{
    ost->curr = NULL;
    pnode_insert(ost->tree, key, value);
}

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

void
objstore_deinit(ObjStore *ost)
{
    pnode_deinit(ost->tree);
    free(ost);
}