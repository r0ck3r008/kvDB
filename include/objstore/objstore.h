#ifndef OBJSTORE_H
#define OBJSTORE_H

#include<stdio.h>

#include"objstore/ptree.h"

/*
 * A type aliased structure encapsulating the Prefix Tree and keeping
 * track of last found Pnode within the tree.
 */
typedef struct ObjStore {
    Pnode *tree, *curr;
} ObjStore;

ObjStore *objstore_init();
ObjStore *objstore_read(FILE *);
void objstore_insert(ObjStore *, char *, char *);
Pnode *objstore_find(ObjStore *, char *);
void objstore_deinit(ObjStore *);

#endif /* OBJSTORE_H */