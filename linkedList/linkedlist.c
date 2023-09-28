#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>

struct Node{
    int data;
    struct Node* next;
};

struct Node* createNode(int data){
    struct Node* new = (struct Node*)malloc(sizeof(struct Node));
    if(new == NULL)
        errx(1, "memory alloc failed!");
    new->data = data;
    new->next = NULL;
    return new;
}

void insertEnd(struct Node** head, int data){
    struct Node* new = createNode(data);
    if(*head == NULL)
        *head = new;
    else{
        struct Node* current = *head;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = new;
    }
}

void debugPrintList(struct Node* head)
{
    struct Node* current = head;
    while(current != NULL)
    {
        printf("%i-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void AllFree(struct Node* head){
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
}

