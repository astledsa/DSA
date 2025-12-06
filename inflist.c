#include <stdio.h>
#include <stdlib.h>
#include "headers/LinkedLists.h"

typedef struct Integers {
    Node* allints;
    Funcs* funcs;
    int currentLength;
}Integers;

Funcs* FuncsInit (Func f) {
    Funcs* node = (Funcs*)malloc(sizeof(Funcs));
    node->f = f; node->next = NULL; return node;
};

void AddFunc (Funcs* Fs, Func f) {
    Funcs* current = Fs;
    while (current->next != NULL) { current = current->next; }
    current->next = (Funcs*)malloc(sizeof(Funcs));
    current->next->f = f;
    current->next->next = NULL;
}

void Apply (Integers* infl, Func f) { 
    if (infl->funcs == NULL) { infl->funcs = FuncsInit(f); } 
    else { AddFunc(infl->funcs, f); }
}

int apply (Funcs* f, int i) {
    if (!f) return i;
    if (f->next == NULL) { 
        int val = f->f(i); 
        return val; 
    }
    int val = f->f(i); return apply(f->next, val);
}

int Get (Integers* infl, int idx) {

    int currentvalue = 1;
    Funcs* fs = infl->funcs;

    int val = apply(fs, currentvalue);
    Node* inflist = initialise(val);
    for (int i = 0; i < idx-1; i++) {
        int val = i + 2;
        val = apply(fs, val);
        append(inflist, val);
        currentvalue = val;
    }

    infl->allints = inflist;
    infl->currentLength = idx;
    return currentvalue;

}

Integers* CreateInts () {
    Integers* ai = (Integers*)malloc(sizeof(Integers));
    ai->currentLength = 0; ai->allints = NULL;
    ai->funcs = NULL; return ai;
}

int Double (int i) { return i + i; }
int square (int i) { return i * i; }

int main () {
    Integers* allints = CreateInts();

    Apply(allints, Double);
    Apply(allints, square);

    int ten = Get(allints, 10);
    print_linked_list(allints->allints);
}

// gcc inflist.c LinkedLists.c -o exec