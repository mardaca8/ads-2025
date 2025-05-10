#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

// Create a new node with data
Node* createNode (int data)
{
    Node *node = (Node*)malloc(sizeof(Node));
    node -> value = data;
    node -> next = NULL;
    node -> prev = NULL;
    return node;
}

// Push a new node with data to the end of the list
void push (Node **head, int data)
{
    Node *newNode = createNode(data);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Node *temp = *head;
        while (temp -> next != NULL)
        {
            temp = temp->next;
        }
        temp -> next = newNode;
        temp -> next -> prev = temp;
    }
}

// Removes the furthest element in the stack, and return it's value.
char popFront (Node **head)
{
    if ((*head) == NULL) return 0;
    if ((*head) -> next == NULL)
    {
        char value = (*head) -> value;
        free(*head);
        (*head) = NULL;
        return value;
    }

    Node *temp = *head;
    while (temp -> next != NULL) temp = temp -> next;
    char value = temp -> value;
    temp -> prev -> next = NULL;
    free(temp);
    return value;
}

// Removes head of stack and replaces it with the second element.
char popBack (Node **head)
{
    if(*head == NULL) return 0;

    char out = (*head) -> value;
    if((*head) -> next == NULL)
    {
        free(*head);
        (*head) = NULL;
        return out;
    }

    Node *temp = *head;
    (*head) = (*head) -> next;
    free(temp);
    return out;
}

// Pushes string of numbers to stack.
void toStack(Node **head, char* data)
{
    int i = 0;
    while(data[i] != '\0')
    {
        if(data[i] < '0' || data[i] > '9') return;
        push(head, data[i] - '0');
        ++i;
    }
}

// Reverse the stack.
void reverse(Node **number){
    Node* temp = NULL;
    while((*number) != NULL) push(&temp, popFront(number));
    while(temp != NULL) push(number, popBack(&temp));
}

// Prints the stacks contents.
void printList (Node *head)
{
    if(head == NULL) return;
    if(head -> next == NULL)
    {
        printf("%d\n", head -> value);
        return;
    }
    while (head -> next != NULL)
    {
        printf("%d", head -> value);
        head = head -> next;
    }
    printf("%d", head -> value);
    printf("\n");
}

// Returns true if num1 is bigger or equal to num2.
bool biggerEqual(Node *num1, Node *num2)
{
    if(num1 == NULL) return false;
    if(num2 == NULL) return true;
    
    Node* temp1 = num1;
    Node* temp2 = num2;
    int c1 = 0, c2 = 0;
    while(temp1 != NULL)
    {
        temp1 = temp1 -> next;
        ++c1;
    }
    while(temp2 != NULL)
    {
        temp2 = temp2 -> next;
        ++c2;
    }
    if(c1 > c2) return true;
    else if(c1 < c2) return false;
    else
    {
        while(num1 != NULL)
        {
            if(num1 -> value > num2 -> value) return true;
            else if(num1 -> value < num2 -> value) return false;
            num1 = num1 -> next;
            num2 = num2 -> next;
        }
    }
    return true;
}

// Adds num2 to num1.
void add(Node **num1, Node *num2) 
{
    reverse(num1);
    reverse(&num2);
    Node* result = NULL;
    Node* temp1 = *num1;
    Node* temp2 = num2;
    int carry = 0;
    while (temp1 != NULL || temp2 != NULL || carry != 0) 
    {
        int sum = carry;
        if (temp1 != NULL) 
        {
            sum += temp1->value;
            temp1 = temp1->next;
        }
        if (temp2 != NULL) 
        {
            sum += temp2->value;
            temp2 = temp2->next;
        }

        push(&result, sum % 10);
        carry = sum / 10;
    }

    *num1 = result;
    reverse(num1);
    reverse(&num2);
}

// Multiplies num1 with num2.
void multiply(Node** num1, Node* num2) 
{
    reverse(num1);
    reverse(&num2);
    Node* result = NULL;
    Node* temp2 = num2;
    reverse(num1);
    reverse(&num2);
    int shift = 0;

    while (temp2 != NULL) 
    {
        Node* temp_result = NULL;
        Node* temp1 = *num1;
        int carry = 0;

        for (int i = 0; i < shift; i++) 
        {
            push(&temp_result, 0);
        }

        while (temp1 != NULL || carry != 0) 
        {
            int product = carry;
            if (temp1 != NULL) 
            {
                product += temp1->value * temp2->value;
                temp1 = temp1->next;
            }
            push(&temp_result, product % 10);
            carry = product / 10;
        }

        add(&result, temp_result);
        temp2 = temp2->next;
        shift++;
    }

    *num1 = result;
    
}

// Adds subtracts num2 from num1.
void subtract(Node** num1, Node** num2)
{
    Node *result = NULL;
    Node *save_num2 = NULL;
    char a = 0;
    char b = 0;
    char temp = 0;
    bool carry = 0;

    while ((*num2) != NULL)
    {
        a = popFront(num1);
        b = popFront(num2);
        push(&save_num2, b);
        if(carry)
        {
            a -= 1;
            if(a < 0)
            {
                a += 10;
                carry = 1;
            }
            else carry = 0;
        }
        temp = a - b;
        if(temp < 0)
        {
            temp += 10;
            carry = 1;
        }
        push(&result, temp);
    }

    while((*num1) != NULL)
    {
        a = popFront(num1);
        temp = a - carry;
        if(temp < 0)
        {
            temp += 10;
            carry = 1;
        }
        else carry = 0;
        push(&result, temp);
    }

    bool zero = true;
    int num;
    while(result != NULL)
    {
        num = popFront(&result);
        if(num != 0) zero = false;
        if(!zero) push(num1, num);
    }

    if(zero) push(num1, 0);
    while (save_num2 != NULL) push(num2, popFront(&save_num2));
}

// Divides num1 from num2.
void divide(Node** num1, Node** num2)
{
    Node *result = NULL;
    Node *temp = createNode(0);
    int temp_num;
    bool first = true;
    int borrow_count = 0;
    while((*num1) != NULL)
    {
        temp_num = popBack(num1);
        if (!first) ++borrow_count;

        if(temp -> value == 0) temp = NULL;

        if(temp_num == 0 && temp == NULL)
        {
            push(&result, 0);
            push(&temp, 0);
        }
        else push(&temp, temp_num);

        while(borrow_count > 1)
        {
            push(&result, 0);
            borrow_count--;
        }

        if(biggerEqual(temp, *num2))
        {
            first = false;
            borrow_count = 0;
            int count = 0;
            while (biggerEqual(temp, *num2))
            {
                subtract(&temp, num2);
                ++count;
            }
            if(count == 10)
            {
                add(&result, createNode(1));
                push(&result, 0);
            }
            else push(&result, count);
        }
    }
    while(result != NULL) push(num1, popBack(&result));
}

//Removes list from memory.
void freeList(Node **head){
    Node* temp;
    while((*head) != NULL)
    {
        temp = (*head);
        (*head) = (*head)->next;
        free(temp);
    }
    head = NULL;
}