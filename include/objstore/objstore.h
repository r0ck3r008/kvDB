#ifndef OBJSTORE_H
#define OBJSTORE_H

#include<stdbool.h>

#include"objstore/ptree.h"

typedef struct ObjStore {
    Pnode *tree, *curr;
} ObjStore;

ObjStore *objstore_init();
void objstore_insert(ObjStore *, char *, char *);
Pnode *objstore_find(ObjStore *, char *);
void objstore_deinit(ObjStore *);

#endif /* OBJSTORE_H */