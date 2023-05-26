#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "linkedlist.h"
#include "dictionary.h"

// Define dictionary path
#define DICTIONARY "dictionaries/large"

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main()
{

    // Load dictionary
    bool loaded = load(DICTIONARY);

    // Exit if dictionary not loaded
    if (!loaded)
    {
        printf("Could not load %s.\n", DICTIONARY);
        return 1;
    }

    // Prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1];
    LinkedList misspelledWords;
    linkedListInit(&misspelledWords);

    char *text;
    printf("Enter file's name: ");
    clearInputBuffer();
    scanf("%s", text);

    // Try to open text
    FILE *file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }
    // Spell-check each word in text
    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while (fread(&c, sizeof(char), 1, file) && isalpha(c))
                    ;

                // Prepare for new word
                index = 0;
            }
        }

        // Ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // Consume remainder of alphanumeric string
            while (fread(&c, sizeof(char), 1, file) && isalnum(c))
                ;

            // Prepare for new word
            index = 0;
        }

        // We must have found a whole word
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';

            // Update counter
            words++;

            // Check word's spelling
            bool misspelled = !check(word);

            // Print word if misspelled
            if (misspelled)
            {
                linkedListInsert(&misspelledWords, word);
                misspellings++;
            }

            // Prepare for next word
            index = 0;
        }
    }

    // Close text
    fclose(file);

    // Determine dictionary's size
    unsigned int n = size();

    // Unload dictionary
    bool unloaded = unload();

    // Abort if dictionary not unloaded
    if (!unloaded)
    {
        printf("Could not unload %s.\n", DICTIONARY);
        return 1;
    }

    // Report benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    linkedListPrint(&misspelledWords);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);

    // Success
    return 0;
}
