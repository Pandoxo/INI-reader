#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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

int isNumber(char* txt)
{
   for(int i = 0; txt[i] !='\0';i++)
   {
    if(!isdigit(txt[i]))
        return 0;
   }
   return 1;

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

void eval(char* arg1,char op,char* arg2)
{
    //int result;
    switch (op)
    {
    case '*':
        //result = arg1 * arg2;
        break;
    case '+':
        printf("%s%s",arg1,arg2);
        break;
    case '-':
        //result = arg1 - arg2;
        break;
    case '/':
        //result = arg1 / arg2; 
        break;
    default:
        printf("\nInvalid operator");
        exit(1);
    }
}

int isKeyValuePair(char* arg)
{
    if(strchr(arg,'.') != NULL && isNumber(arg) == 0)
        return 1;
    return 0;
}


int main(int argc, char* argv[])
{
   

    FILE *fptr;
    char path[256];
    int size_sections = 20;
    int i = -1;
    Section* sections = malloc(size_sections * sizeof(Section));
    if (argv[1] != NULL)
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
                KeyValuePair pair;
                
                char* tokPtr;
                tokPtr = strtok(line, " ");
                pair.key = strdup(tokPtr);
                tokPtr = strtok(NULL," ");//=
                tokPtr = strtok(NULL," \n");
                pair.value = strdup(tokPtr);
                


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

    bool exp = false;
    if (strcmp(argv[2],"expression") == 0)
    {
        char* expPtr;
        expPtr =  strtok(argv[3]," ");
        char* var1 = strdup(expPtr);
        expPtr = strtok(NULL," ");
        char* operator = strdup(expPtr);
        expPtr = strtok(NULL," ");
        char* var2 = strdup(expPtr);

        printf("%s %s %s\n",var1,operator,var2);
        invalidSymbols(var1,"var1");
        invalidSymbols(var2,var2);


    }else{

        char* tokPtr = strtok(argv[2], ".");
        char* sectionName = tokPtr;
        tokPtr = strtok(NULL, ".");
        char* keyName = tokPtr;
        
        ifmissing(sectionName, "section");
        ifmissing(keyName, "key");
        invalidSymbols(sectionName, "section");
        invalidSymbols(keyName, "key");

        char* value = findValue(sections,sections->sizeOfPairs,sectionName,keyName);
        printf("Searched value: %s",value );
    }
    return 0;
}
