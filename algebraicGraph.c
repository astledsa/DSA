#include <stdio.h>
#include <stdlib.h>

typedef struct Vertex {
    int value;
    struct Vertex* next;
}Vertex;

typedef struct Edge {
    Vertex* source;
    Vertex* target;
    struct Edge* next;
}Edge;

typedef struct {
    Vertex* vertices;
    Edge* edges;
}Graph;

void print_edge (Edge* edge) {
    printf("Source: %d ", edge->source->value);
    printf("Target: %d\n", edge->target->value);
}

void print_edge_list (Edge* head) {
    print_edge(head);
    while(head->next) {
        head = head->next;
        print_edge(head);
    }
}

void print_vertex_list (Vertex* head) {
    printf("%d ", head->value);
    while(head->next) {
        head = head->next;
        printf("%d ", head->value);
    }
    printf("\n");
}

void print_graph (Graph* G) {
    printf("Vertices: ");
    print_vertex_list(G->vertices);
    printf("\n");
   
    if (!G->edges) {
        printf("No Edges\n");
        return;
    } else {
        printf("Edges:\n");
        print_edge_list(G->edges);
   }
}

Vertex* copy_vertex(Vertex* head) {
    if (!head) {
        return NULL; 
    }

    Vertex* new_head = NULL;
    Vertex* current = head;
    Vertex* prev = NULL;

    while (current) {
        Vertex* new_node = malloc(sizeof(Vertex));
        if (!new_node) {
        return NULL;
        }

        new_node->value = current->value;

        if (!new_head) {
            new_head = new_node;
        }

        if (prev) {
            prev->next = new_node;
        }
        prev = new_node;
        current = current->next;
    }
  return new_head;
}

Edge* copy_edge (Edge* head) {
    if (!head) {
        return NULL; 
    }

    Edge* new_head = NULL;
    Edge* current = head;
    Edge* prev = NULL;

    while (current) {
        Edge* new_node = malloc(sizeof(Edge));
        if (!new_node) {
        return NULL;
        }

        new_node->source = current->source;
        new_node->target = current->target;

        if (!new_head) {
            new_head = new_node;
        }

        if (prev) {
            prev->next = new_node;
        }
        prev = new_node;
        current = current->next;
    }
  return new_head;
}

Vertex* removeDuplicates_vertex (Vertex* vertex_head) {
    if (!vertex_head || !vertex_head->next) {
        return vertex_head; 
    }
    Vertex* current = vertex_head;
    while (current->next) {
        if (current->value == current->next->value) {
            Vertex* temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
    return vertex_head; 
}

Edge* removeDuplicates_edge (Edge* edge_head) {
    if (!edge_head || !edge_head->next) {
        return edge_head; 
    }
    Edge* current = edge_head;
    while (current->next) {
        if (
            (current->source->value == current->next->source->value && current->target->value == current->next->target->value) ||
            (current->target->value == current->next->source->value && current->source->value == current->next->target->value)
        ) {
            Edge* temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
    return edge_head;
}

Vertex* vertex_set_union (Vertex* v1, Vertex* v2) {
    if (!v1) {
        return v2;
    }
    Vertex* new_vertex = copy_vertex(v1);
    Vertex* current = new_vertex;
    while(current->next) {
        current = current->next;
    }
    current->next = copy_vertex(v2);
    return removeDuplicates_vertex(new_vertex);
}

Edge* edge_set_union (Edge* e1, Edge* e2) {
    if (!e1) {
        return e2;
    }
    if (!e2) {
        return e1;
    }
    Edge* new_edge = copy_edge(e1);
    Edge* current = new_edge;
    while(current->next) {
        current = current->next;
    }
    current->next = copy_edge(e2);
    return removeDuplicates_edge(new_edge);
}

Edge* create_edge (Vertex* v1, Vertex* v2) {
    Edge* new_edge = (Edge*)malloc(sizeof(Edge));
    new_edge->source = v1;
    new_edge->target = v2;
    new_edge->next = NULL;

    return new_edge;
}

void append_edge (Edge* linked_list, Edge* new_edge) {
    while(linked_list->next) {
        linked_list = linked_list->next;
    }
    linked_list->next = new_edge;
}

Graph* create_graph_vertex (int value) {
    Vertex* new_vertex = (Vertex*)malloc(sizeof(Vertex));
    new_vertex->value = value;

    Graph* new_graph = (Graph*)malloc(sizeof(Graph));
    new_graph->vertices = new_vertex;
    new_graph->edges = NULL;

    return new_graph;
}

Graph* Overlay (Graph* g1, Graph* g2) {
    Graph* new_graph = (Graph*)malloc(sizeof(Graph));
    new_graph->vertices = vertex_set_union(g1->vertices, g2->vertices);
    new_graph->edges = edge_set_union(g1->edges, g2->edges);

    return new_graph;
}

Graph* Connect (Graph* g1, Graph* g2) {
    Vertex* current_g1_vertex = g1->vertices;
    Vertex* current_g2_vertex = g2->vertices;
    Edge* new_edge_list = create_edge(NULL, NULL);

    while(current_g1_vertex) {
        while(current_g2_vertex) {
            if (new_edge_list->source == NULL && new_edge_list->target == NULL) {
                new_edge_list->source = current_g1_vertex;
                new_edge_list->target = current_g2_vertex;
            } else {
                Edge* new_edge = create_edge (current_g1_vertex, current_g2_vertex);
                append_edge(new_edge_list, new_edge);
            }
            current_g2_vertex = current_g2_vertex->next;
        }
        current_g2_vertex = g2->vertices;
        current_g1_vertex = current_g1_vertex->next;
    }

    Graph* new_graph = (Graph*)malloc(sizeof(Graph));
    new_graph->vertices = vertex_set_union(g1->vertices, g2->vertices);
    new_graph->edges = edge_set_union(edge_set_union(g1->edges, g2->edges), new_edge_list);

    return new_graph;
}

int main () {
    Graph* G1 = create_graph_vertex(12);
    Graph* G2 = create_graph_vertex(23);
    Graph* G4 = create_graph_vertex(37);
    Graph* G5 = create_graph_vertex(45);
    Graph* G3 = Connect(G1, G2);
    Graph* G6 = Connect(G4, G5);
    Graph* G7 = Overlay(G3, G6);

    print_graph(G7);
}

// gcc algebraicGraph.c -o exec