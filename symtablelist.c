
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

struct symtable {
    int size;
    struct node *head;
    struct node *tail;
};

struct node {
    char *key;
    void *value;
    struct node *next;
};

SymTable_T SymTable_new() {
    SymTable_T SymTable = (SymTable_T)malloc(sizeof(struct symtable));
    assert(SymTable != NULL);
    SymTable->size = 0;
    return SymTable;
};

void SymTable_free(SymTable_T SymTable) {
    if (SymTable == NULL) return;
    Node current_node = SymTable->head;
    Node next_node;
    while (current_node -> next != NULL) {
        next_node = current_node -> next;
        free(current_node -> key);
        free(current_node);
        current_node = next_node;
    }

    free(SymTable);
}

unsigned int SymTable_getLength(SymTable_T SymTable) {
    assert(SymTable != NULL);
    return SymTable->size;
}

int SymTable_put(SymTable_T SymTable, const char *pcKey, const void *pvValue) {
    assert(SymTable != NULL);
    assert(pcKey != NULL);
    if (SymTable -> head == NULL) {
        Node new_node = new_binding(pcKey, pvValue);
        SymTable -> head = new_node;
        SymTable -> tail = new_node;
        SymTable -> size++;
        return 1;
    }
    Node current_node = SymTable->head;
    Node previous_node = SymTable->head;

    while (current_node != NULL) {

        if (strcmp(current_node -> key, pcKey) == 0) return 0;
        previous_node = current_node;
        current_node = current_node -> next;

    }

    Node new_node = new_binding(pcKey, pvValue);
    previous_node -> next = new_node;
    SymTable -> size++;
    return 1;
}

int SymTable_remove(SymTable_T SymTable, const char *pcKey) {
    assert(SymTable != NULL);
    assert(pcKey != NULL);
    Node current_node = SymTable->head;
    Node previous_node = SymTable->head;

    if (strcmp(SymTable -> head -> key, pcKey) == 0) {
        Node tmp = SymTable->head;
        SymTable -> head = tmp -> next;
        free(tmp -> key);
        free(tmp);
        SymTable -> size--;
        return 1;
    }

    while (current_node != NULL) {
        if (strcmp(current_node -> key, pcKey) == 0) {
            previous_node -> next = current_node -> next;
            free(current_node -> key);
            free(current_node);
            SymTable -> size--;
            return 1;
        }
        previous_node = current_node;
        current_node = current_node -> next;
    }

    return 0;
}

int SymTable_contains(SymTable_T SymTable, const char *pcKey) {
    assert(SymTable != NULL);
    assert(pcKey != NULL);

    Node current_node = SymTable->head;
    for (; current_node != NULL; current_node = current_node -> next) {
        if (strcmp(current_node -> key, pcKey) == 0) {return 1;}
    }


    return 0;
}

void *SymTable_get(SymTable_T SymTable, const char *pcKey) {
    assert(SymTable != NULL);
    assert(pcKey != NULL);

    Node current_node = SymTable->head;
    for (; current_node != NULL; current_node = current_node -> next) {
        if (strcmp(current_node -> key, pcKey) == 0) {return current_node -> value;}
    }
    return NULL;
}

void SymTable_map(SymTable_T SymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra) {
    assert(SymTable != NULL);
    assert(pfApply != NULL);

    Node current_node = SymTable->head;
    for (; current_node != NULL; current_node = current_node -> next) {
        pfApply(current_node -> key, current_node -> value, pvExtra);
    }
}

void pfApply(const char *pcKey, void *pvValue, void *pvExtra) {
    int type = *((int *)pvExtra);
    switch (type) {
        case 1:
            //print int
            int *val = pvValue;
            printf("%s -> %d\n",pcKey,*val);
            break;
        default:
            assert(0);
    }
}


Node new_binding(char *key, void *value) {
    Node binding = (Node)malloc(sizeof(struct node));
    assert(binding != NULL);
    binding->key = strdup(key);
    binding->value = value;
    return binding;
}