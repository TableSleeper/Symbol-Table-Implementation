#include <stdio.h>
#include "symtable.h"

int main(void) {

    SymTable_T table = SymTable_new();
    int x;
    int *num = &x;
    *num = 2;


    SymTable_put(table,"meow",num);
    int does = SymTable_contains(table,"meow");
    int how =*(int*) SymTable_get(table,"meow");
    printf("Does it exist: %d\n",does);
    printf("Whats the value: %d\n",how);
    SymTable_free(table);

    return 0;
}