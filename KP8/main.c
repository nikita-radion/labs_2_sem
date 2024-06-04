#include <stdio.h>
#include <stdlib.h>
#include "node.h"

int main() {
    Node* head = NULL;

    head = insertNode(head, 10);
    head = insertNode(head, 8);
    head = insertNode(head, 4);

    Iterator* it = createIterator(head);
    printf("nodes: ");
    Node* node;
    while ((node = next(it)) != NULL) {
        printf("%zu ", node->data);
    }
    printf("\n");

    int count = countNodes(head);
    printf("nymber of nodes: %d\n", count);

    head = deleteNode(head, 10);
    printf("delete node 10: ");
    printList(head);

    head = clearList(head, 4);
    printf("list cheked: ");
    printList(head);

    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    free(it);

    return 0;
}