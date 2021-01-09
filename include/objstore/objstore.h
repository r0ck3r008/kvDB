#ifndef OBJSTORE_H
#define OBJSTORE_H

#include<stdio.h>

#include"objstore/ptree.h"

/*
 * A type aliased structure encapsulating the Prefix Tree.
 */
typedef struct ObjStore {
    Pnode *tree;
} ObjStore;

ObjStore *objstore_init();
ObjStore *objstore_read(FILE *);
void objstore_insert(ObjStore *, char *, char *);
Pnode *objstore_find(ObjStore *, char *);
void objstore_deinit(ObjStore *);

#endif /* OBJSTORE_H */