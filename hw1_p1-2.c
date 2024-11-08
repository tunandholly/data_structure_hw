#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Checking whether the symbol is operand or not.
int isOperand(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Converting postfix to prefix expression in C
char* postfixToPrefix(const char* postfix) {
    char* stack[MAX];
    int top = -1;

    for (int i = 0; i < strlen(postfix); i++) {
        if (isOperand(postfix[i])) {
            char* op = (char*)malloc(2);
            op[0] = postfix[i];
            op[1] = '\0';
            stack[++top] = op;
        } else {
            char* op1 = stack[top--];
            char* op2 = stack[top--];

            char* expression = (char*)malloc(strlen(op1) + strlen(op2) + 2);
            sprintf(expression, "%c%s%s", postfix[i], op2, op1);

            stack[++top] = expression;
        }
    }

    return stack[top];
}

int main() {
    char postfix[MAX], *prefix;

    
    scanf("%s", postfix);

    

    prefix = postfixToPrefix(postfix);

    printf("%s", prefix);

    return 0;
}
