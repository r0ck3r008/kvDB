#ifndef KEYLIST_H
#define KEYLIST_H

typedef struct KeyList {
    char key[512], *value;
    struct KeyList *next;
} KeyList;

KeyList *keylist_init(char *);
KeyList *keylist_prepend(KeyList *, char *, char *);
void keylist_deinit(KeyList *);

#endif /* KEYLIST_H */