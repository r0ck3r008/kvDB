#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include "objstore/objstore.h"

ObjStore *
inputDictionary(FILE *f)
{
    ObjStore *ost = objstore_init();
    char *line = NULL;
    size_t n = 0;
    char buf[512] = {0};
    while(getline(&line, &n, f) != -1) {
        snprintf(buf, n, "%s", line);
        char *key = strtok(buf, ":");
        char *value = strtok(NULL, ":");
        objstore_insert(ost, key, value);

        free(line);
        line = NULL;
        n = 0;
        explicit_bzero(buf, sizeof(char) * 512);
    }

    return ost;
}

int
main(int argc, char **argv)
{
    if(argc!=2) {
        fprintf(stderr, "[-] Usage: ./example.out <path-to-dict-file>\n");
        _exit(1);
    }
    FILE *f = NULL;
    if((f=fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "[-] Error in opening file %s: %s\n", argv[1], strerror(errno));
        _exit(1);
    }
    ObjStore *ost = inputDictionary(f);
    Pnode *ret = NULL;
    if((ret=objstore_find(ost, "abate")) != NULL)
        printf("Meaning: %s\n", ret->value);
    else
        printf("Not Found!\n");
    objstore_deinit(ost);
    fclose(f);
}