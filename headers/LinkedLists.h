#include <stdio.h>
#include <stdlib.h>

typedef int (*Func)(int);

typedef struct Funcs {
    Func f;
    struct Funcs* next;
}Funcs;

typedef struct Node{
    int value;
    struct Node* next;
}Node;

Funcs* FuncsInit (Func f);
void AddFunc (Funcs* LL, Func f);

Node* initialise (int number);
void print_linked_list (Node* current);
void append (Node* LL, int number);
void removeByValue (Node** LL, int value);
void pop (Node* LL);
int getidx (Node* current, int number);