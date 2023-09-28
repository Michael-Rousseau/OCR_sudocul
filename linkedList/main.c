#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"


int main() {
    struct Node* head = NULL;

    insertEnd(&head, 1);
    insertEnd(&head, 2);
    insertEnd(&head, 3);

    printf("Linked List: ");
    debugPrintList(head);

    AllFree(head);

    return 0;
}
