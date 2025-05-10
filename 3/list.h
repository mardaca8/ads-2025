#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct Node{
    char value;
    struct Node *next;
    struct Node *prev;
}Node;

void push (Node **head, int data);
char popFront (Node **head);
char popBack (Node **head);

void printList(Node *head);
void freeList(Node **head);
void subtract(Node **num1, Node **num2);
void toStack(Node **head, char *data);
void divide(Node **num1, Node **num2);
void reverse(Node **number);
void add(Node** num1, Node* nufixem2);
void multiply(Node** num1, Node* num2);

#endif