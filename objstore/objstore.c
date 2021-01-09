#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>

#include"objstore/objstore.h"

char *
rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

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

ObjStore *
objstore_read(FILE *f)
{
    ObjStore *ost = objstore_init();
    char *line = NULL;
    size_t n = 0;
    char buf[512] = {0};
    while(getline(&line, &n, f) != -1) {
        snprintf(buf, n, "%s", line);
        char *key = strtok(buf, ":");
        key = rtrim(key);
        char *value = strtok(NULL, ":");
        objstore_insert(ost, key, value);

        free(line);
        line = NULL;
        n = 0;
        explicit_bzero(buf, sizeof(char) * 512);
    }
    free(line);

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
    return pnode_find(ost->tree, key);
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