
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
