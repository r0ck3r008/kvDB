#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include "objstore/objstore.h"

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
    ObjStore *ost = objstore_read(f);
    Pnode *ret = NULL;
    if((ret=objstore_find(ost, "abate")) != NULL)
        printf("Meaning: %s\n", ret->value);
    else
        printf("Not Found!\n");
    objstore_deinit(ost);
    fclose(f);
}