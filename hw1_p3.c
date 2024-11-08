#include <stdio.h>
#include <stdlib.h>

// Comparator function to use in sorting
int comparator(const void* p1, const void* p2) {
    const long long int (*x)[3] = p1;
    const long long int (*y)[3] = p2;

    return (*x)[2] - (*y)[2];
}

// Initialization of parent[] and rank[] arrays
void makeSet(long long int parent[], int rank[], int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

// Function to find the parent of a node
long long int findParent(long long int parent[], int component) {
    if (parent[component] == component)
        return component;

    return parent[component] = findParent(parent, parent[component]);
}

// Function to unite two sets
void unionSet(long long int u, long long int v, long long int parent[], int rank[], int n) {
    // Finding the parents
    u = findParent(parent, u);
    v = findParent(parent, v);

    if (rank[u] < rank[v]) {
        parent[u] = v;
    } else if (rank[u] > rank[v]) {
        parent[v] = u;
    } else {
        parent[v] = u;

        // Since the rank increases if
        // the ranks of two sets are same
        rank[u]++;
    }
}

// Function to find the MST
long long int kruskalAlgo(int n, long long int edge[][3]) {
    // First, we sort the edge array in ascending order
    // so that we can access minimum distances/cost
    qsort(edge, n, sizeof(edge[0]), comparator);

    long long int parent[n];
    int rank[n];

    // Function to initialize parent[] and rank[]
    makeSet(parent, rank, n);

    // To store the minimum cost
    long long int minCost = 0;

    for (int i = 0; i < n; i++) {
        long long int v1 = findParent(parent, edge[i][0]);
        long long int v2 = findParent(parent, edge[i][1]);
        long long int wt = edge[i][2];

        // If the parents are different that
        // means they are in different sets so
        // union them
        if (v1 != v2) {
            unionSet(v1, v2, parent, rank, n);
            minCost += wt;
        }
    }

    return minCost;
}

// Driver code
int main() {
    int V, E;
    scanf("%d %d", &V, &E);

    long long int edge[E][3];
    for (int i = 0; i < E; i++) {
        scanf("%lld %lld %lld", &edge[i][0], &edge[i][1], &edge[i][2]);
    }

    long long int result = kruskalAlgo(E, edge);

    printf("%lld\n", result);

    return 0;
}
