#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE_NUM 10001

void Initialize_sets(int node_num);
void Process_Order();
int Find(int target);
void Same(int target1, int target2);
void Union(int target1, int target2);

int parent[MAX_NODE_NUM];

int main(int argc, char* argv[]) {
    int case_num;
    scanf("%d", &case_num);
    while (case_num--) {
        int node_num, operation_num;
        scanf("%d %d", &node_num, &operation_num);
        // Initial node_num個set(with only one element)
        Initialize_sets(node_num);
        // 開始跑共operation_num次的操作
        for (int i = 0; i < operation_num; i++) Process_Order();
    }
    return 0;
}

void Process_Order() {
    int target1, target2;
    // C字串結束要有一個 '/0'之類的 所以至少要開到6rrr
    char operation[6];
    scanf("%s", operation);
    if (strcmp(operation, "find") == 0) {
        scanf("%d", &target1);
        printf("%d\n", Find(target1));
    } else if (strcmp(operation, "same") == 0) {
        scanf("%d %d", &target1, &target2);
        Same(target1, target2);
    } else if (strcmp(operation, "union") == 0) {
        scanf("%d %d", &target1, &target2);
        Union(target1, target2);
    }
}

void Initialize_sets(int node_num) {
    for (int i = 0; i < node_num; i++) parent[i] = -1;
}

int Find(int target) {
    return (parent[target] < 0 ? target : (parent[target] = Find(parent[target])));
}

void Union(int target1, int target2) {
    int root1 = Find(target1), root2 = Find(target2);
    if (root1 == root2) return;
    // -2 < -1 高度是root1 > root2
    if (parent[root1] < parent[root2]) {
        parent[root2] = root1;
    }
    // -1 > -2 高度是root1 < root2
    else if (parent[root1] > parent[root2]) {
        parent[root1] = root2;
    }
    // The height is the same, the height will increase 1 after merge
    else {
        parent[root2] = root1;
        parent[root1]--;
    }
}

void Same(int target1, int target2) {
    printf(Find(target1) == Find(target2) ? "true\n" : "false\n");
}
