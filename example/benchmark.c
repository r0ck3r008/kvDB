#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<ctype.h>
#include<errno.h>

#include "objstore/objstore.h"

#define MAXLINES 120000

char *
rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char **
readin_file(FILE *f)
{
    if(fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "[-] Error in setting resetting: %s\n", strerror(errno));
        _exit(1);
    }
    char **words = calloc(MAXLINES, sizeof(char *));
    if(words==NULL) {
        fprintf(stderr, "[-] Error in allocating memeory!\n");
        _exit(1);
    }

    size_t n = 0;
    int i = 0;
    while(getline(&words[i], &n, f)!=-1) {
        words[i] = rtrim(words[i]);
        i++;
        n = 0;
    }

    return words;
}

void
free_file(char **words)
{
    for(int i=0; i<MAXLINES; i++) {
        if(words[i]!=NULL)
            free(words[i]);
    }

    free(words);
}

int
benchmark(ObjStore *ost, char **words)
{
    Pnode *ret = NULL;
    int count = 0;
    for(int i=0; i<MAXLINES; i++) {
        if(words[i]!=NULL && (ret = objstore_find(ost, words[i])) != NULL)
            count++;
    }

    return count;
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
    ObjStore *ost = objstore_read(f);
    char **words = readin_file(f);

    int count = 0;
    clock_t start = clock();
    count = benchmark(ost, words);
    clock_t end = clock();
    fprintf(stderr, "[!] It took %lf seconds for %d records!\n", ((double)(end-start))/CLOCKS_PER_SEC, count);

    objstore_deinit(ost);
    free_file(words);
    fclose(f);
}