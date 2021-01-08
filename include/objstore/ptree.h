#ifndef PTREE_H
#define PTREE_H

/*
 * A type aliased structure which represents a single node in Prefix Tree.
 */
typedef struct Pnode {
    char *key, *value;
    struct Pnode *child[26];
} Pnode;

Pnode *pnode_init(char *, char *);
Pnode *pnode_insert(Pnode *, char *, char *);
Pnode *pnode_find(Pnode *, char *);
void pnode_deinit(Pnode *);

#endif /* PTREE_H */