#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
keylist_prepend(KeyList *list, char *key, char *value)
{
    /* If list is NULL, create a new list and make it have the key */
    if(list==NULL) {
        list = keylist_init(key);
    } else if(list->value!=NULL) {
        /* If the node is already complete, make a new one */
        KeyList *new = keylist_init(key);
        new->next=list;
        list = new;
    } else {
        /* If list is non-empty, append to old key */
        strcat(list->key, key);
    }

    if(value!=NULL)
        /* This is as far as this key goes */
        list->value = value;

    return list;
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