#ifndef PTREE_H
#define PTREE_H

#include<stdbool.h>

typedef struct Pnode {
    char *key, *value;
    struct Pnode *child[26];
} Pnode;

typedef struct Ptree {
    Pnode *root;
} Ptree;

Pnode *pnode_init(char *, char *);
void pnode_insert(Pnode *, char *, char *);
Pnode *pnode_find(Pnode *, char *);
void pnode_deinit(Pnode *);

Ptree *ptree_init();
void ptree_insert(Ptree *, char *, char *);
Pnode *ptree_find(Ptree *, char *);
void ptree_deinit(Ptree *);

#endif /* PTREE_H */