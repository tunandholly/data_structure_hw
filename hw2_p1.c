#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

// Define F-heap node structure
typedef struct FibonacciNode {
    int key;
    int value;
    struct FibonacciNode *parent;
    struct FibonacciNode *child;
    struct FibonacciNode *left;
    struct FibonacciNode *right;
    int degree;
    int marked;
} FibonacciNode;

// Define F-heap structure
typedef struct FibonacciHeap {
    FibonacciNode *min;
    int size;
} FibonacciHeap;

// Function prototypes
FibonacciHeap* createFibonacciHeap();
FibonacciNode* createFibonacciNode(int key, int value);
void insert(FibonacciHeap *heap, int key, int value);
void extractMin(FibonacciHeap *heap);
void deleteNode(FibonacciHeap *heap, int key, int value);
void decreaseKey(FibonacciHeap *heap, int key, int value, int decreaseBy);
void cascadingCut(FibonacciHeap *heap, FibonacciNode *node);
void consolidate(FibonacciHeap *heap);
void link(FibonacciNode *min, FibonacciNode *max);
FibonacciNode* findNode(FibonacciNode *start, int key, int value);
void printMin(FibonacciHeap *heap);
void quit(FibonacciHeap *heap);
void destroyFibonacciHeap(FibonacciHeap *heap);
void freeNodes(FibonacciNode *start);

int main() {
    FibonacciHeap *fibHeap = createFibonacciHeap();
    char command[20];
    int key, value, decreaseBy;

    while (1) {
        
        if (scanf("%s", command) != 1) {
            printf("Error reading input.\n");
            break;
        }

        // Process the command
        if (strcmp(command, "insert") == 0) {
            scanf("%d %d", &key, &value);
            insert(fibHeap, key, value);
        } else if (strcmp(command, "extract") == 0) {
            extractMin(fibHeap);
        } else if (strcmp(command, "delete") == 0) {
            scanf("%d %d", &key, &value);
            deleteNode(fibHeap, key, value);
        } else if (strcmp(command, "decrease") == 0) {
            scanf("%d %d %d", &key, &value, &decreaseBy);
            decreaseKey(fibHeap, key, value, decreaseBy);
        } else if (strcmp(command, "printMin") == 0) {
            printMin(fibHeap);
        } else if (strcmp(command, "quit") == 0) {
            quit(fibHeap);
            break;
        } else {
            printf("Invalid command. Please try again.\n");
        }
    }

    return 0;
}

// Function to create an empty F-heap
FibonacciHeap* createFibonacciHeap() {
    FibonacciHeap *heap = (FibonacciHeap *)malloc(sizeof(FibonacciHeap));
    if (heap == NULL) {
        printf("Error: Unable to create Fibonacci heap\n");
        exit(EXIT_FAILURE);
    }
    heap->min = NULL;
    heap->size = 0;
    return heap;
}

// Function to create a Fibonacci node
FibonacciNode* createFibonacciNode(int key, int value) {
    FibonacciNode *node = (FibonacciNode *)malloc(sizeof(FibonacciNode));
    if (node == NULL) {
        printf("Error: Unable to create Fibonacci node\n");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->value = value;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    node->degree = 0;
    node->marked = 0;
    return node;
}

// Function to insert a node into the F-heap
void insert(FibonacciHeap *heap, int key, int value) {
    FibonacciNode *node = createFibonacciNode(key, value);

    if (heap->min == NULL) {
        heap->min = node;
    } else {
        // Add the new node to the root list
        node->right = heap->min->right;
        node->left = heap->min;
        heap->min->right->left = node;
        heap->min->right = node;

        // Update min if necessary
        if (node->key < heap->min->key) {
            heap->min = node;
        }
    }

    heap->size++;

    // Check if the new node has the same key as an existing node
    if (node->key == heap->min->key && node != heap->min) {
        link(heap->min, node);
    }
}


// Function to free a single node without freeing its children
void freeSingleNode(FibonacciNode *node) {
    free(node);
}

// Function to extract the minimum node from the F-heap
void extractMin(FibonacciHeap *heap) {
    if (heap->min == NULL) {
        printf("Heap is empty. Cannot extract minimum.\n");
        return;
    }

    FibonacciNode *minNode = heap->min;
    FibonacciNode *child = minNode->child;
    FibonacciNode *firstChild = child;

    // Remove minNode from the root list
    if (minNode->right == minNode) {
        heap->min = NULL;
    } else {
        minNode->left->right = minNode->right;
        minNode->right->left = minNode->left;
        heap->min = minNode->right;
    }

    // Add minNode's children to the root list
    if (child != NULL) {
        do {
            FibonacciNode *nextChild = child->right;
            child->left = heap->min->left;
            child->right = heap->min;
            heap->min->left->right = child;
            heap->min->left = child;
            child->parent = NULL;
            child = nextChild;
        } while (child != firstChild);
    }

    // Consolidate the heap
    consolidate(heap);

    heap->size--;
    printf("(%d)%d\n", minNode->key, minNode->value);
    freeSingleNode(minNode);  // Use the new function here
    heap->min = NULL;  // Set heap->min to NULL after freeing minNode
}

// Function to delete a node with a specified key and value
void deleteNode(FibonacciHeap *heap, int key, int value) {
    FibonacciNode *node = findNode(heap->min, key, value);

    if (node == NULL) {
        printf("Node with key %d and value %d not found.\n", key, value);
        return;
    }

    // Remove the node from the root list
    if (node->right == node) {
        heap->min = NULL;
    } else {
        node->left->right = node->right;
        node->right->left = node->left;
        if (heap->min == node) {
            heap->min = node->right;
        }
    }

    // Add node's children to the root list
    FibonacciNode *child = node->child;

    if (child != NULL) {
        do {
            child->parent = NULL;
            child = child->right;
        } while (child != node->child);
    }

    // Reset child pointer
    node->child = NULL;

    // If the deleted node has a parent, remove the child link
    if (node->parent != NULL) {
        if (node->parent->child == node) {
            node->parent->child = node->right;
            if (node->right == node) {
                node->parent->child = NULL;
            }
        }

        // Update sibling links
        node->left->right = node->right;
        node->right->left = node->left;

        // If the deleted node was marked, mark the parent
        if (node->parent->marked == 0) {
            node->parent->marked = 1;
        } else {
            cascadingCut(heap, node->parent);
        }
    }

    freeSingleNode(node);
}

// Function to decrease the key of a node in the F-heap
void decreaseKey(FibonacciHeap *heap, int key, int value, int decreaseBy) {
    FibonacciNode *node = findNode(heap->min, key, value);

    if (node == NULL) {
        printf("Node with key %d and value %d not found.\n", key, value);
        return;
    }

    // For deletion, we don't need to check if the new key is less than the current key
    if (decreaseBy != INT_MIN) {
        if (node->key - decreaseBy < node->key) {
            node->key -= decreaseBy;

            FibonacciNode *parent = node->parent;

            if (parent != NULL && node->key < parent->key) {
                // Perform cascading cut
                cascadingCut(heap, node);
            }

            if (node->key < heap->min->key) {
                heap->min = node;
            }

            return;
        } else {
            return;
        }
    }

    // For deletion, just extract the node
    extractMin(heap);
}

// Function to perform cascading cut
void cascadingCut(FibonacciHeap *heap, FibonacciNode *node) {
    FibonacciNode *parent = node->parent;

    // If the node has a parent, cut the edge between the node and its parent
    if (parent != NULL) {
        if (node->right == node) {
            parent->child = NULL;
        } else {
            parent->child = node->right;
            node->left->right = node->right;
            node->right->left = node->left;
        }

        // Add the cut node to the root list
        node->left = heap->min->left;
        node->right = heap->min;
        heap->min->left->right = node;
        heap->min->left = node;

        // Update heap->min if necessary
        if (node->key < heap->min->key) {
            heap->min = node;
        }

        node->parent = NULL;
        node->marked = 0;

        // Perform cascading cut on the parent
        if (parent->marked == 0) {
            parent->marked = 1;
        } else {
            cascadingCut(heap, parent);
        }
    }
}

// Function to consolidate the heap
void consolidate(FibonacciHeap *heap) {
    int maxDegree = (int)(log(heap->size) / log(1.618)) + 1;  // Golden ratio

    FibonacciNode **degreeTable = (FibonacciNode **)malloc(sizeof(FibonacciNode *) * (maxDegree + 1));
    if (degreeTable == NULL) {
        printf("Error: Unable to allocate memory for the degree table\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= maxDegree; i++) {
        degreeTable[i] = NULL;
    }

    // Consolidate nodes with the same degree
    FibonacciNode *current = heap->min;
    FibonacciNode *first = heap->min;
    do {
        FibonacciNode *next = current->right;
        int degree = current->degree;

        while (degreeTable[degree] != NULL) {
            FibonacciNode *other = degreeTable[degree];

            // Ensure current has a smaller key
            if (current->key > other->key) {
                FibonacciNode *temp = current;
                current = other;
                other = temp;
            }

            // Link other as a child of current
            link(current, other);

            degreeTable[degree] = NULL;
            degree++;
        }

        degreeTable[degree] = current;

        current = next;
    } while (current != first);

    // Find the new minimum node
    heap->min = NULL;
    for (int i = 0; i <= maxDegree; i++) {
        if (degreeTable[i] != NULL) {
            if (heap->min == NULL || degreeTable[i]->key < heap->min->key) {
                heap->min = degreeTable[i];
            }
        }
    }

    free(degreeTable);
}

// Function to link two Fibonacci nodes
void link(FibonacciNode *min, FibonacciNode *max) {
    // Remove max from the root list
    max->left->right = max->right;
    max->right->left = max->left;

    // Make max a child of min
    max->left = min;
    max->right = min->right;
    min->right->left = max;
    min->right = max;

    min->degree++;

    max->marked = 0;
}

// Function to find a node with a specified key and value
FibonacciNode* findNode(FibonacciNode *start, int key, int value) {
    FibonacciNode *current = start;
    FibonacciNode *result = NULL;

    do {
        if (current->key == key && current->value == value) {
            result = current;
            break;
        }

        FibonacciNode *childResult = findNode(current->child, key, value);
        if (childResult != NULL) {
            result = childResult;
            break;
        }

        current = current->right;
    } while (current != start);

    return result;
}

// Function to terminate the program and free memory
void quit(FibonacciHeap *heap) {
    destroyFibonacciHeap(heap);
}

void destroyFibonacciHeap(FibonacciHeap *heap) {
    if (heap != NULL) {
        if (heap->min != NULL) {
            // Free nodes starting from the min node
            freeNodes(heap->min);
        }
        free(heap);
    }
}

// Function to free nodes in the Fibonacci heap
void freeNodes(FibonacciNode *start) {
    FibonacciNode *current = start;
    do {
        FibonacciNode *next = current->right;
        if (current->child != NULL) {
            freeNodes(current->child);
        }
        free(current);
        current = next;
    } while (current != start);
}
