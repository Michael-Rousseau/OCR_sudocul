#pragma once
struct Node;


struct Node* createNode(int data);

void insertEnd(struct Node** head, int data);


void debugPrintList(struct Node* head);

void AllFree(struct Node* head);
