
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

int N = 100;

// Hash table
node *table[100]; 

char *lowerWord(char *word)
{
    for (int i = 0; i < strlen(word); i++)
    {
        word[i] = tolower(word[i]);
    }
    return word;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char *temp = lowerWord(word);

    int hashIndex = hash(temp);

    node *p = table[hashIndex];
    while (p)
    {
        if (p->word == temp)
        {
            return true;
        }

        p = p->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    char t[LENGTH + 1];
    strcpy(t, word);
    for (int i = 0; t[i] != '\0'; i++)
    {
        t[i] = tolower(t[i]);
        sum += (int)t[i];
    }

    return sum % N;
}

// loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char words[LENGTH + 1];
    while (fscanf(file, "%s", words) != EOF)
    {
        node *temp = malloc(sizeof(node));
        if (temp == NULL)
        {
            return false;
        }
        strcpy(temp->word, words);
        temp->next = NULL;

        int hashValue = hash(words);

        if (table[hashValue] == NULL)
        {
            table[hashValue] = temp;
        }
        else
        {
            node *q = table[hashValue];
            // set node temp to its location
            temp->next = q->next;
            q = temp;
        }
    }

    if (fscanf(file, "%s", words) == EOF)
    {
        fclose(file);
        return true;
    }

    fclose(file);
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int num = 0;
    for (int i = 0; i < N; i++)
    {
        node *p = table[i];
        while (p != NULL)
        {
            num++;
            p = p->next;
        }
    }
    return num;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *p = table[i];
        while (p != NULL)
        {
            node *t = p;
            p = p->next;
            free(t);
        }
        free(p);
    }
    return true;
}
