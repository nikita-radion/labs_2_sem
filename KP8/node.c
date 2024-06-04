#include <stdio.h>
#include <stdlib.h>
#include "node.h"

Iterator* createIterator(Node* head) {
    Iterator* it = (Iterator*)malloc(sizeof(Iterator));
    it->current = head;
    return it;
}

Node* next(Iterator* it) {
    if (it->current == NULL) {
        return NULL;
    }
    Node* node = it->current;
    it->current = it->current->next;
    return node;
}

Node* createNode(size_t data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* insertNode(Node* head, size_t data) {
    Node* newNode = createNode(data);
    newNode->next = head;
    return newNode;
}

Node* deleteNode(Node* head, size_t key) {
    Node* temp = head, *prev = NULL;

    if (temp != NULL && temp->data == key) {
        head = temp->next;
        free(temp);
        return head;
    }

    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return head;

    prev->next = temp->next;
    free(temp);

    return head;
}

int countNodes(Node* head) {
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void printList(Node* head) {
    if (head == NULL) {
        printf("\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        printf("%zu ", current->data);
        current = current->next;
    }
    printf("\n");
}

Node* clearList(Node* head, size_t key) {
    Node* temp = head;

    while (temp != NULL) {
        if (temp->data == key) {
            Node* current = head;
            while (current != NULL) {
                Node* next = current->next;
                free(current);
                current = next;
            }
            return NULL;
        } else {
            temp = temp->next;
        }
    }

    return head;
}