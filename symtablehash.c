#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

struct symtable {
    Node* buckets;
    size_t size;
    unsigned int bucketCount;
};

struct node {
    char *key;
    void *value;
    struct node *next;
};


//unsigned int steps[8] = {509,1021,2053,4093,8191,16381,32771,65221};
unsigned int hashenator(const char *pcKey)     {
    unsigned long hash = 0;
    int c;

    while ((c = *(unsigned char*) pcKey++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}


SymTable_T SymTable_new(void) {
    Node* buckets = calloc(509, sizeof(Node*));
    SymTable_T SymTable = malloc(sizeof(struct symtable));
    assert(SymTable != NULL);
    assert(buckets != NULL);
    SymTable->buckets = buckets;
    SymTable->size = 0;
    SymTable->bucketCount = 509;
    return SymTable;
}

void SymTable_free(SymTable_T SymTable) {
    assert(SymTable != NULL);
    for (unsigned int i =0;i < SymTable->bucketCount;i++) {
        if (SymTable->buckets[i] != NULL) {
            Node current = SymTable->buckets[i];
            while (current != NULL) {
                void* tmp = current -> next;
                free(current->key);
                free(current);
                current = tmp;
            }
        }
    }
    free(SymTable->buckets);
    free(SymTable);
    printf("Symtable fred\n");
}

unsigned int SymTable_getLength(SymTable_T SymTable) {
    assert(SymTable != NULL);
    return SymTable->size;
}

int SymTable_put(SymTable_T SymTable, const char *pcKey, const void *pvValue) {
    assert(SymTable != NULL);

    unsigned int hash = hashenator(pcKey);
    unsigned int tableSize = SymTable_getLength(SymTable);
    unsigned int boundary = SymTable->bucketCount;
    unsigned int index = hash % boundary;
    if ((double)SymTable->size / SymTable->bucketCount > 0.75) {
        //Expand hash
    }
    //Initialize the first node in that index
    if (SymTable->buckets[index] == NULL) {
        Node first = malloc(sizeof(struct node));
        assert(first != NULL);
        first->key = strdup(pcKey);
        first->value = pvValue;
        first->next = NULL;
        SymTable->buckets[index] = first;
        SymTable->size++;
        return 1;
    }
    Node current = SymTable->buckets[index];
    int exists = 0;
    while (current!=NULL) {
        assert(strcmp(current->key, pcKey) == 0);
        current = current->next;
    }
    Node tmp = SymTable->buckets[index];
    // na apothikeuso to ptr to prohgoumenou prwtou

    Node new_node = malloc(sizeof(struct node));
    assert(new_node != NULL);
    new_node->key = strdup(pcKey);
    new_node->value = (void *)pvValue;
    //na kanw ena neo prwto

    SymTable->buckets[index] = new_node;
    new_node->next = tmp;
    SymTable->size++;
    //na to valo prwto, kai to next tou na deixnei sto ex-prwto

    return 1;
}

int SymTable_remove(SymTable_T SymTable, const char *pcKey) {
    assert(SymTable != NULL);
    assert(pcKey != NULL);
    unsigned int hash = hashenator(pcKey);
    unsigned int index = hash % SymTable->bucketCount;
    Node *current = &SymTable->buckets[index];
    while (*current != NULL) {
        if (strcmp((*current)->key, pcKey) == 0) {
            remove_Node(current);
            SymTable->size--;
            return 1;
        }
        current = &(*current)->next;

    }
    return 0;
}

int SymTable_contains(SymTable_T SymTable, const char *pcKey){
    assert(SymTable != NULL);
    assert(pcKey != NULL);
    unsigned int hash = hashenator(pcKey);
    unsigned int index = hash % SymTable->bucketCount;
    Node *current = &SymTable->buckets[index];
    while (*current != NULL) {
        if (strcmp((*current)->key, pcKey) == 0) {
            return 1;
        }
        current = &(*current)->next;

    }
    return 0;
}

void *SymTable_get(SymTable_T SymTable, const char *pcKey){
    assert(SymTable != NULL);
    assert(pcKey != NULL);
    unsigned int hash = hashenator(pcKey);
    unsigned int index = hash % SymTable->bucketCount;
    Node *current = &SymTable->buckets[index];
    while (*current != NULL) {
        if (strcmp((*current)->key, pcKey) == 0) {
            return (*current)->value;
        }
        current = &(*current)->next;

    }
    assert(!"Element with pcKey not found");
}

void SymTable_map(SymTable_T SymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra) {
    assert(SymTable != NULL);
    for (int i =0;i < SymTable->bucketCount;i++) {
        Node *current = &SymTable->buckets[i];
        while (*current != NULL) {
            pfApply((*current)->key,(*current)->value,pvExtra);
            current = &(*current)->next;

        }
    }

}

void pfApply(const char *pcKey, void *pvValue, void *pvExtra) {
    *(int*)pvValue += *(int*)pvExtra;
}

Node new_binding(char *key, void *value) {

}

void remove_Node(Node* nodeptr) {
    assert(nodeptr != NULL);
    //save node thats next of the node the ptr is pointing to
    //remove current, make Node* point to next
    Node toBeRemoved = *nodeptr;
    *nodeptr = toBeRemoved->next;
    free(toBeRemoved->key);
    free(toBeRemoved);
}