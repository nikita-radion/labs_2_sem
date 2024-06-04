#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct _node {
    struct _node* left;
    struct _node* right;
    char* value;
} Node;

Node* parseExpression(char** expression);
Node* parseTerm(char** expression);
Node* parseFactor(char** expression);

Node* newNode(char* value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->left = NULL;
    node->right = NULL;
    node->value = (char*)malloc((strlen(value) + 1) * sizeof(char));
    strcpy(node->value, value);
    return node;
}

char* nextToken(char** expression) {
    while (isspace(**expression)) (*expression)++;
    if (**expression == '\0') return NULL;

    int length = 0;
    if (isdigit(**expression)) {
        while (isdigit((*expression)[length])) length++;
    } else {
        length = 1;
    }

    char* token = (char*)malloc((length + 1) * sizeof(char));
    strncpy(token, *expression, length);
    token[length] = '\0';
    (*expression) += length;

    return token;
}

Node* parseFactor(char** expression) {
    char* token = nextToken(expression);
    if (token == NULL) return NULL;
    Node* node;
    if (strcmp(token, "(") == 0) {
        node = parseExpression(expression);
        free(token);
        token = nextToken(expression);
        if (token == NULL || strcmp(token, ")") != 0) {
            exit(1);
        }
    } else if (strcmp(token, "-") == 0) {
        node = newNode(token);
        node->right = parseFactor(expression);
    } else {
        node = newNode(token);
    }
    free(token);
    return node;
}

Node* parsePower(char** expression) {
    Node* node = parseFactor(expression);
    char* token;
    while ((token = nextToken(expression)) != NULL) {
        if (strcmp(token, "^") == 0) {
            Node* right = parseFactor(expression);
            Node* parent = newNode(token);
            parent->left = node;
            parent->right = right;
            node = parent;
        } else {
            (*expression)--;
            free(token);
            break;
        }
        free(token);
    }
    return node;
}

Node* parseTerm(char** expression) {
    Node* node = parsePower(expression);
    char* token;
    while ((token = nextToken(expression)) != NULL) {
        if (strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
            Node* right = parsePower(expression);
            Node* parent = newNode(token);
            parent->left = node;
            parent->right = right;
            node = parent;
        } else {
            (*expression)--;
            free(token);
            break;
        }
        free(token);
    }
    return node;
}

Node* parseExpression(char** expression) {
    Node* node = parseTerm(expression);
    char* token;
    while ((token = nextToken(expression)) != NULL) {
        if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0) {
            Node* right = parseTerm(expression);
            Node* parent = newNode(token);
            parent->left = node;
            parent->right = right;
            node = parent;
        } else {
            (*expression)--;
            free(token);
            break;
        }
        free(token);
    }
    return node;
}

void printTree(Node* node, int space) {
    if (node == NULL) return;

    space += 1;

    printTree(node->right, space);

    printf("\n");
    for (int i = 1; i < space; i++)
        printf(" ");
    printf("%s\n", node->value);

    printTree(node->left, space);
}

void freeTree(Node* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node->value);
    free(node);
}

void simplifyPowers(Node* node) {
    if (node == NULL) return;
    simplifyPowers(node->left);
    simplifyPowers(node->right);
    if (strcmp(node->value, "^") == 0) {
        char* end;
        long base = strtol(node->left->value, &end, 10);
        if (*end != '\0') return;  
        long exponent = strtol(node->right->value, &end, 10);
        if (*end != '\0') return;  
        long result = 1;
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
        free(node->value);
        node->value = (char*)malloc(21 * sizeof(char));  
        sprintf(node->value, "%ld", result);
        freeTree(node->left);
        freeTree(node->right);
        node->left = NULL;
        node->right = NULL;
    }
}

void printExpressionHelper(Node* node, int isRoot, char* parentValue, int parentIsEmptyPlus) {
    if (node == NULL) return;
    int isUnaryMinus = (strcmp(node->value, "-") == 0 && node->right != NULL && node->left == NULL);
    int isEmptyPlus = (strcmp(node->value, "+") == 0 && node->left == NULL && node->right != NULL);
    int needParentheses = (!isRoot && !parentIsEmptyPlus && (isUnaryMinus || 
                      ((strcmp(node->value, "*") == 0 || strcmp(node->value, "/") == 0) && 
                      (strcmp(parentValue, "+") == 0 || strcmp(parentValue, "-") == 0))));
    if (needParentheses) printf("(");
    printExpressionHelper(node->left, 0, node->value, isEmptyPlus);
    if (!isEmptyPlus) printf("%s", node->value);
    printExpressionHelper(node->right, 0, node->value, isEmptyPlus);
    if (needParentheses) printf(")");
}

void printExpression(Node* node) {
    printExpressionHelper(node, 1, NULL, 0);
}

int main() {
    char expression[100];
    printf(">>> ");
    fgets(expression, 100, stdin);
    char* p = expression;
    Node* root = parseExpression(&p);
    printExpression(root);
    printf("\n");
    simplifyPowers(root);
    printExpression(root);
    printTree(root, 0);
    printf("\n");
    freeTree(root);
    return 0;
}