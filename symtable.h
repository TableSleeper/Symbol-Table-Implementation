#pragma once

typedef struct symtable *SymTable_T;

SymTable_T SymTable_new(void);

void SymTable_free(SymTable_T SymTable);

unsigned int SymTable_getLength(SymTable_T SymTable);

int SymTable_put(SymTable_T SymTable, const char *pcKey, const void *pvValue);

int SymTable_remove(SymTable_T SymTable, const char *pcKey);

int SymTable_contains(SymTable_T SymTable, const char *pcKey);

void *SymTable_get(SymTable_T SymTable, const char *pcKey);

void SymTable_map(SymTable_T SymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra);

void pfApply(const char *pcKey, void *pvValue, void *pvExtra);

typedef struct node *Node;

Node new_binding(char *key, void *value);

void remove_Node(Node* nodeptr);

