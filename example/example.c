#include<stdio.h>

#include "objstore/objstore.h"

int main(int argc, char **argv)
{
    ObjStore *ost = objstore_init();
    objstore_insert(ost, "naman", "arora");
    objstore_insert(ost, "sakshu", "arora");
    objstore_insert(ost, "nanan", "arora");
    if (objstore_find(ost, "sakshu") != NULL)
        printf("Found!\n");
    objstore_deinit(ost);
}