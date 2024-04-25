#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_LEVEL 10

typedef struct Node {
    int value;
    int level;
    struct Node* next_node[MAX_LEVEL];
}Node;

Node* create_node (double value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->level = 1;
    node->value = value;
    for(int i = 0; i < MAX_LEVEL; i++) {
        node->next_node[i] = NULL;
    }
    return node;
}

Node* initialise_node () {
    Node* skip_list_head = create_node(-INFINITY);
    skip_list_head->level = MAX_LEVEL;
    return skip_list_head;
}
 
Node* get_previous_node (Node* head, double value) {
    for (int i = head->level-1; i >= 0; i--) {
        if (head->next_node[i] != NULL && head->next_node[i]->value < value) {
            return get_previous_node(head->next_node[i], value);
        }
    }
    return head;
}

int randomZeroOrOne() {
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
    
    float randomNum = (float)rand() / RAND_MAX;
    return randomNum < 0.5 ? 0 : 1;
}

int random_level_of_node (int max_level) {
    int level = 1;
    while(level < max_level) {
        if (randomZeroOrOne()) {
            level += 1;
        } else {return level;}
    }
    return level;
}

void insert_node (Node* skip_list_head, double value) {
    Node* new_node = create_node(value);
    new_node->level = random_level_of_node(MAX_LEVEL);

    if (skip_list_head->value < value) {
        Node* previous_node = get_previous_node(skip_list_head, value);
        for (int i = new_node->level-1; i >= 0; i--) {
            new_node->next_node[i] = previous_node->next_node[i];
            previous_node->next_node[i] = new_node;
        }
    }
}

void print_node_info (Node* node) {
    printf("Value: %d\n", node->value);
    printf("Level: %d\n", node->level);
    for (int i = node->level-1; i >= 0; i--) {
        printf("Level %d: ", i);
        if (node->next_node[i] == NULL) {
            printf("NULL\n");
        } else {
            printf("%d\n", node->next_node[i]->value);
        }
    }
    printf("\n");
}

void print_list (Node* skiplist) {
    Node* current = skiplist->next_node[0];
    while(current != NULL) {
        printf("%d ", current->value);
        current = current->next_node[0];
    }
}

void print_distribution (Node* skiplist) {
    Node* current = skiplist;
    printf("\n");
    while(current != NULL) {
        for (int i = current->level-1; i >= 0; i--) {
            printf("%s ", "|");
        }
        printf("\n");
        current = current->next_node[0];
    }
    printf("\n");
}

int main () {
    Node* skiplist = initialise_node();

    insert_node(skiplist, -1);
    insert_node(skiplist, 2);
    insert_node(skiplist, -45);
    insert_node(skiplist, 0);

    print_list(skiplist);
    print_distribution(skiplist);
}