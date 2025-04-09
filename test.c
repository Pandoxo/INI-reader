#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Section;
struct Array;

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

typedef struct Section {
    char *name;
    KeyValuePair* pairs;
    int numberOfPairs;
    int sizeOfPairs;
} Section;

void invalidSymbols(char* txt, char* name)
{
    for (int i = 0; txt[i] != '\0'; i++)
    {
        char c = txt[i];
        if (!((c > 47 && c < 58) || (c > 64 && c < 133) || (c > 96 && c < 123) || (c == '-')))
        {
            printf("invalid %s: %s\n", name, txt);
            exit(1);
        }
    }
}

void ifmissing(char* arg, char* name)
{
    if (arg == NULL)
    {
        printf("%s missing", name);
        exit(1);
    }
}

int addSection(Section** a, int i, int capacity, Section* element)
{
    if (i >= capacity)
    {
        capacity *= 2;
        Section* b = realloc(*a, capacity * sizeof(Section));
        if (!b)
        {
            perror("realloc");
            exit(1);
        }
        *a = b;
    }
    (*a)[i] = *element;
    return capacity;
}

int addPair(KeyValuePair** a, int i, int capacity, KeyValuePair* element)
{
    if (i >= capacity)
    {
        capacity *= 2;
        KeyValuePair* b = realloc(*a, capacity * sizeof(KeyValuePair));
        if (!b)
        {
            perror("realloc");
            exit(1);
        }
        *a = b;
    }
    (*a)[i] = *element;
    return capacity;
}

char* findValue(Section* sections,int size,char* section, char* key)
{
    for (int i = 0; i< size; i++)
    {
        if(strcmp(sections[i].name,section)==0)
        {
            for(int j = 0; j< sections[i].numberOfPairs; j++)
            {
                KeyValuePair pair = sections[i].pairs[j]; 
                char* key2 = pair.key;
                if (strcmp(key,key2)==0)
                {
                    return pair.value;
                }
            }
        }
    }
    return "Not Found";

}

char* readLine(FILE* fptr)
{
    int bufsize = 64;
    int pos = 0;
    char* buffer = malloc(bufsize);
    if (!buffer) { perror("malloc"); exit(1); }

    int c;
    while ((c = fgetc(fptr)) != EOF && c != '\n')
    {
        buffer[pos++] = c;
        if (pos >= bufsize)
        {
            bufsize *= 2;
            buffer = realloc(buffer, bufsize);
            if (!buffer) { perror("realloc"); exit(1); }
        }
    }

    if (c == EOF && pos == 0) {
        free(buffer);
        return NULL;
    }

    buffer[pos] = '\0';
    return buffer;
}

int main(int argc, char* argv[])
{
    FILE *fptr;
    char path[256];
    int size_sections = 20;
    int i = -1;
    Section* sections = malloc(size_sections * sizeof(Section));

    strcpy(path, argv[1]);
    fptr = fopen(path, "r");

    if (fptr != NULL)
    {
        char* line;
        while ((line = readLine(fptr)) != NULL)
        {
            if (line[0] == '[')
            {
                char* tokSectionPtr = strtok(line, "[]");

                Section sec;
                sec.name = strdup(tokSectionPtr);
                sec.numberOfPairs = 0;
                sec.sizeOfPairs = 5;

                ifmissing(sec.name, "section");
                invalidSymbols(sec.name, "section");

                i++;
                size_sections = addSection(&sections, i, size_sections, &sec);

                KeyValuePair* pairsArray = malloc(sec.sizeOfPairs * sizeof(KeyValuePair));
                sections[i].pairs = pairsArray;
            }
            else if (line[0] == '\0' || line[0] == ';')
            {
                free(line);
                continue;
            }
            else
            {
                char* tokPtr2 = strtok(line, " ");
                //har* equalSign = strtok(NULL, " ");
                char* valPtr = strtok(NULL, "\n");

                KeyValuePair pair;
                pair.key = strdup(tokPtr2);
                pair.value = strdup(valPtr);

                ifmissing(pair.key, "key");
                ifmissing(pair.value, "value");
                invalidSymbols(pair.key, "key");
                invalidSymbols(pair.value, "value");

                sections[i].sizeOfPairs = addPair(&sections[i].pairs, sections[i].numberOfPairs, sections[i].sizeOfPairs, &pair);
                sections[i].numberOfPairs++;
            }
            free(line);
        }
        fclose(fptr);
    }
    else
    {
        printf("file open unsuccessful");
        return 1;
    }

    char* tokPtr = strtok(argv[2], ".");
    char* sectionName = tokPtr;
    tokPtr = strtok(NULL, ".");
    char* keyName = tokPtr;

    ifmissing(sectionName, "section");
    ifmissing(keyName, "key");
    invalidSymbols(sectionName, "section");
    invalidSymbols(keyName, "key");

    for (int s = 0; s <= i; s++)
    {
        if (strcmp(sections[s].name, sectionName) == 0)
        {
            for (int p = 0; p < sections[s].numberOfPairs; p++)
            {
                if (strcmp(sections[s].pairs[p].key, keyName) == 0)
                {
                    printf("%s\n", sections[s].pairs[p].value);
                    return 0;
                }
            }
            printf("key not found\n");
            return 1;
        }
    }

    printf("section not found\n");
    return 1;
}
