// gcc tensor.c -o exec 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct GradNode {
    double value;
    struct GradNode* next;
}GradNode;

typedef struct Tensor{
    double value;
    double power;
    double gradient;
    char* creation_operation;
    GradNode* backward_grads;
    struct Tensor* parents[2];
}Tensor;

GradNode* initialise_grad (double value) {
    GradNode* new_node = (GradNode*)malloc(sizeof(GradNode));
    new_node->value = value;
    new_node->next = NULL;

    return new_node;
}

double Sum (GradNode* head) {
    double sum = 0;
    GradNode* current = head;
    while (current != NULL) {
        sum += current->value;
        current = current->next;
    }
    return sum;
}

void append_grad (GradNode* head, GradNode* grad) {
    while(head->next) {
        head = head->next;
    }
    head->next = grad;
}

void print_grad_list (GradNode* head) {
    printf("%f ", head->value);
    while(head->next) {
        head = head->next;
        printf("%f ", head->value);
    }
    printf("\n");
}

Tensor* tensor (double number) {
    Tensor* new_tensor = (Tensor*)malloc(sizeof(Tensor));

    new_tensor->value = number;
    new_tensor->gradient = 1.0f;
    new_tensor->power = 1.0f;
    new_tensor->parents[0] = NULL; 
    new_tensor->parents[1] = NULL; 
    new_tensor->backward_grads = NULL;
    new_tensor->creation_operation = NULL;

    return new_tensor;
}

Tensor* ElementryOperations (Tensor* a, Tensor* b, char* operation) {
    Tensor* new_tensor = tensor(0);
    new_tensor->creation_operation = operation;
    new_tensor->parents[0] = a;
    new_tensor->parents[1] = b;

    if (strcmp(operation, "add") == 0) {
        new_tensor->value = a->value + b->value;
        return new_tensor;
    }
    if (strcmp(operation, "sub") == 0) {
        new_tensor->value = a->value - b->value;
        return new_tensor;
    }
    if (strcmp(operation, "div") == 0) {
        if (b->value == 0.0f) {
            printf("Division by zero");
            return;
        }
        new_tensor->value = a->value / b->value;
        return new_tensor;
    }
    if (strcmp(operation, "mult") == 0) {
        new_tensor->value = a->value * b->value;
        return new_tensor;
    }

    return NULL;
}

Tensor* Add (Tensor* a, Tensor* b) {
    return ElementryOperations(a, b, "add");
}

Tensor* Sub (Tensor* a, Tensor* b) {
    return ElementryOperations(a, b, "sub");
}

Tensor* Mult (Tensor* a, Tensor* b) {
    return ElementryOperations(a, b, "mult");
}

Tensor* Div (Tensor* a, Tensor* b) {
    return ElementryOperations(a, b, "div");
}

Tensor* Sin (Tensor* a) {
    Tensor* new_tensor = tensor(sin(a->value));
    new_tensor->creation_operation = "sin";
    new_tensor->parents[0] = a;

    return new_tensor;
}

Tensor* Cos (Tensor* a) {
    Tensor* new_tensor = tensor(cos(a->value));
    new_tensor->creation_operation = "cos";
    new_tensor->parents[0] = a;

    return new_tensor;
}

Tensor* Log (Tensor* a) {
    Tensor* new_tensor = tensor(log(a->value));
    new_tensor->creation_operation = "log";
    new_tensor->parents[0] = a;

    return new_tensor;
}

Tensor* Power (Tensor* a, double power) {
    Tensor* new_tensor = tensor(pow(a->value, power));
    new_tensor->creation_operation = "pow";
    new_tensor->parents[0] = a;
    new_tensor->power = power;

    return new_tensor;
}

void Backward (Tensor* z, double backward_gradient) {
    GradNode* new_grad = initialise_grad(backward_gradient);
    if (z->backward_grads == NULL) {
        z->backward_grads = new_grad;
    } else {
        append_grad (z->backward_grads, new_grad);
    }

    z->gradient = Sum(z->backward_grads);

    if (z->parents[0] == NULL && z->parents[1] == NULL) {
        return;
    }

    if (strcmp(z->creation_operation, "add") == 0) {
        Backward(z->parents[0], z->gradient * (1.0f));
        Backward(z->parents[1], z->gradient * (1.0f));
    }

    if (strcmp(z->creation_operation, "sub") == 0) {
        Backward(z->parents[0], z->gradient * (1.0f));
        Backward(z->parents[1], z->gradient * (-1.0f));
    }

    if (strcmp(z->creation_operation, "mult") == 0) {
        Backward(z->parents[0], z->gradient * (z->parents[1]->value));
        Backward(z->parents[1], z->gradient * (z->parents[0]->value));
    }

    if (strcmp(z->creation_operation, "div") == 0) {
        if (z->parents[1]->value == 0) {
            printf("Division by zero");
            return;
        }
        Backward(z->parents[0], z->gradient * (1/z->parents[1]->value));
        Backward(z->parents[1], z->gradient * (-(z->parents[0]->value/pow(z->parents[1]->value, 2.0f))));
    }

    if (strcmp(z->creation_operation, "sin") == 0) {
        Backward(z->parents[0], z->gradient * (cos(z->parents[0]->value)));
    }

    if (strcmp(z->creation_operation, "cos") == 0) {
        Backward(z->parents[0], z->gradient * (-sin(z->parents[0]->value)));
    }

    if (strcmp(z->creation_operation, "log") == 0) {
        if (z->parents[0]->value == 0) {
            printf("Division by zero error");
            return;
        } 
        Backward(z->parents[0], z->gradient * (1/(z->parents[0]->value)));
    }

    if (strcmp(z->creation_operation, "pow") == 0) {
        Backward(z->parents[0], z->gradient * (z->power * pow(z->parents[0]->value, z->power-1.0f)));
    }
}

int main () {
    Tensor* x = tensor(2);
    Tensor* y = tensor(3);
    Tensor* z = Log(Add(Cos(Mult(x, y)),Mult(x, Sin(y)))); // log(cos(xy) + xsin(y))

    Backward(z, 1.0f);
    printf("%f\n", x->gradient);
    printf("%f\n", y->gradient);
}
