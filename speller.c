#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "linkedlist.h"
#include "dictionary.h"

#define DICTIONARY "dictionaries/large"

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main()
{
    bool loaded = load(DICTIONARY);

    if (!loaded)
    {
        printf("Could not load %s.\n", DICTIONARY);
        return 1;
    }

    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1];
    LinkedList misspelledWords;
    linkedListInit(&misspelledWords);

    char *text;
    printf("Enter file's name: ");
    clearInputBuffer();
    scanf("%s", text);

    FILE *file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            word[index] = c;
            index++;

            if (index > LENGTH)
            {
                while (fread(&c, sizeof(char), 1, file) && isalpha(c))
                    ;

                // Prepare for new word
                index = 0;
            }
        }
        else if (isdigit(c))
        {
            while (fread(&c, sizeof(char), 1, file) && isalnum(c))
                ;
            // Prepare for new word
            index = 0;
        }
        else if (index > 0)
        {
            word[index] = '\0';

            words++;

            bool misspelled = !check(word);

            if (misspelled)
            {
                linkedListInsert(&misspelledWords, word);
                misspellings++;
            }
            // Prepare for next word
            index = 0;
        }
    }

    fclose(file);

    unsigned int n = size();

    bool unloaded = unload();

    if (!unloaded)
    {
        printf("Could not unload %s.\n", DICTIONARY);
        return 1;
    }

    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    linkedListPrint(&misspelledWords);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);

    return 0;
}
