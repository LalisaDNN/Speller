#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 45

typedef struct Node
{
    char word[LENGTH + 1];
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
} LinkedList;

void linkedListInit(LinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
}

Node *createNode(const char *word)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Failed to allocate memory for the new node.\n");
        return NULL;
    }
    strncpy(newNode->word, word, LENGTH);
    newNode->word[LENGTH] = '\0';
    newNode->next = NULL;
    return newNode;
}

void linkedListInsert(LinkedList *list, const char *word)
{
    Node *newNode = createNode(word);
    if (newNode == NULL)
    {
        return;
    }
    if (list->head == NULL)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

void linkedListPrint(const LinkedList *list)
{
    Node *current = list->head;
    while (current != NULL)
    {
        printf("%s ", current->word);
        current = current->next;
    }
    printf("\n");
}

void linkedListFree(LinkedList *list)
{
    Node *current = list->head;
    while (current != NULL)
    {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->tail = NULL;
}
