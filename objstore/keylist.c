#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include"objstore/keylist.h"

KeyList *
keylist_init(char *key)
{
    KeyList *list = calloc(1, sizeof(KeyList));
    if(list==NULL) {
        fprintf(stderr, "[-] KEYLIST: Malloc: Error in allocating memeory!\n");
        _exit(1);
    }
    list->key = key;

    return list;
}

KeyList *
prepend(KeyList *list, char *key)
{
    KeyList *new = keylist_init(key);
    if(list!=NULL)
        new->next = list;

    return new;
}

void
keylist_deinit(KeyList *list)
{
    KeyList *curr = list->next;
    while(list->next!=NULL) {
        list->next = curr->next;
        free(curr);
        curr = list->next;
    }
    free(list);
}