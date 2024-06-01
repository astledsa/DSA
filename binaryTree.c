#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FAILED,
    SUCCESS
}SEARCH_RESULT;

typedef struct {
    int key;
    char* name;
}Entry;

typedef struct Node {
    struct Node* right;
    struct Node* left;
    Entry* node_entry;
}Node;

typedef struct {
    Node* node;
    SEARCH_RESULT status;
}NodeResult;

typedef struct {
    Node* root;
    char* mode;
}Tree;

Entry* create_entry (int key, char* name) {
    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->name = name;

    return new_entry;
}

Node* initialise_node (int key, char* name) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    Entry* new_entry = create_entry(key, name);

    new_node->node_entry = new_entry;
    new_node->right = NULL;
    new_node->left = NULL;

    return new_node;
}

Tree* initialise_tree (int key, char* name, char* mode) {
    Tree* new_tree = (Tree*)malloc(sizeof(Tree));
    new_tree->root = initialise_node(key, name);
    new_tree->mode = mode;

    return new_tree;
}

void Insert (Tree* Tree, int key, char* name) {
    Node* new_node = initialise_node(key, name);
    Node* current = Tree->root;
    
    while (current) {
        if (strcmp(Tree->mode, "keys") == 0) {
            if (current->node_entry->key > new_node->node_entry->key) {
                if (current->left == NULL) {break;}
                current = current->left;
            } else if (current->node_entry->key < new_node->node_entry->key) {
                if (current->right == NULL) {break;}
                current = current->right;
            } else {
                printf("Duplicate values: %d and %d", current->node_entry->key, new_node->node_entry->key);
                break;
            }
        } else if (strcmp(Tree->mode, "names") == 0) {
            if (strcmp(current->node_entry->name, new_node->node_entry->name) == 1) {
                if (current->left == NULL) {break;}
                current = current->left;
            } else if (strcmp(current->node_entry->name, new_node->node_entry->name) == -1) {
                if (current->right == NULL) {break;}
                current = current->right;
            } else {
                printf("Duplicate values: %s and %s", current->node_entry->name, new_node->node_entry->name);
                break;
            }
        }
    }
    
    if (new_node->node_entry->key > current->node_entry->key) {
        current->right = new_node;
    } else {
        current->left = new_node;
    }
}

NodeResult* Search (Tree* tree, int key) {
    NodeResult* result = (NodeResult*)malloc(sizeof(NodeResult));
    Node* current = tree->root;
    while (current) {
        if (current->node_entry->key == key) {
            result->node = current;
            result->status = SUCCESS;
            return result;
        } else if (current->node_entry->key < key) {
            if (current->right == NULL) {break;}
            current = current->right;
        } else {
            if (current->left == NULL) {break;}
            current = current->left;
        }
    }
    result->node = NULL;
    result->status = FAILED;

    return result;
}

void print_right_nodes (Tree* tree) {
    Node* current = tree->root;
    while(current) {
        printf("%d ", current->node_entry->key);
        current = current->right;
    }
    printf("\n");
}

void print_left_nodes (Tree* tree) {
    Node* current = tree->root;
    while(current) {
        printf("%d ", current->node_entry->key);
        current = current->left;
    }
    printf("\n");
}

int main () {
    Tree* tree = initialise_tree(11, "Astle", "keys");

    Insert(tree, 10, "Astle");
    Insert(tree, 1, "Sarah");
    Insert(tree, 12, "Bennett");
    Insert(tree, 14, "Jude");
    Insert(tree, 50, "Nicole");

    NodeResult* res = Search(tree, 45);
    if (res->status == SUCCESS) {
        printf("%s\n", res->node->node_entry->name);
    } else {
        printf("Error in finding the node\n");
    }
}

// gcc binaryTree.c -o exec