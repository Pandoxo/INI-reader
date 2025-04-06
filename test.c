#include <stdio.h>
#include<stdlib.h>
#include <string.h>


struct Section;
struct Array;

typedef struct {
    char *key;
    char *value;
} KeyValuePair;


typedef struct 
{
    Section* data;
    size_t used;
    size_t size;
} Array;

typedef struct {
    char *name;
    Array *pairs;
    int numberOfPairs;
} Section;

void initArray(Array* a,size_t initialSize)
{
    a->data = malloc(initialSize * sizeof(Section));
    a->used =0;
    a->size = initialSize;
}
void insertArray(Array* a,Section data)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->data = realloc(a->data,a->size * sizeof(Section));
    }
    a->data[a->used++] = data;
}

void invalidSymbols(char* txt,char* name)
{
    
    for(int i = 0; txt[i] != '\0';i++)
    {
        char c = txt[i];
        if ( !((c>47 && c<58) || (c>64 && c<133) || (c>96 &&c<123) || (c=='-')) )
        {
            printf("invalid %s: %s\n",name,txt);
            exit(1);
        }
    }
}

void ifmissing(char* arg,char* name)
{
    if(arg == NULL)
    {
        printf("%s missing",name);
        exit(1);
    }
}


int main(int argc, char* argv[])
{
    FILE *fptr;
    char content[63];
    char path[20];

    //Initialize dynamic array
    Array sections;
    initArray(&sections,100);
    
    strcpy(path,argv[1]);
    fptr = fopen(path,"r");
    
    int i = -1; //numer of section
    
    char* tokPtr;
    tokPtr = strtok(argv[2],".");
    char* section = tokPtr;
    tokPtr = strtok(NULL,",");
    char* key= tokPtr;
    // printf("%s\n",section);
    // printf("%s\n",key);
    // printf("%s\n\n",path);
    if (fptr != NULL)
    {
        printf("\nfile opened\n");
        char* readSection;
        while (fgets(content,63,fptr))
        {
            char firstLetter = content[0];    
            char* readkey = NULL;
            char* readValue = NULL;
            if (firstLetter == '[')
            {
                //Reading section
                Array atributes;
                initArray(&atributes,5);

                char* tokSectionPtr = strtok(content,"[]");
                Section sec;
                sec.name = strdup(tokSectionPtr);

                ifmissing(sec.name,"section");
                invalidSymbols(sec.name,"section");
                sec.numberOfPairs = 0;

                insertArray(&sections,sec);
                i++;
                readSection = strdup(tokSectionPtr);
                ifmissing(readSection,"section");
                invalidSymbols(readSection,"section");

            }else if (firstLetter == '\n')
            {
                continue;
            }
            else
            {
                KeyValuePair pair;

                //Reading key and value
                char* tokPtr2;
                
                tokPtr2 = strtok(content," ");

                pair.key = strdup(tokPtr2);
                readkey = strdup(tokPtr2);
                tokPtr2 = strtok(NULL," "); // "="
                tokPtr2 = strtok(NULL," \n");
                readValue = strdup(tokPtr2);
                pair.value = strdup(tokPtr2);
                
                ifmissing(pair.key,"key");
                ifmissing(pair.value,"value");
                invalidSymbols(pair.key,"key");
                invalidSymbols(pair.value,"value");
                // insertArray(sections.data[i].pairs
                ifmissing(readkey,"key");
                ifmissing(readValue,"value");
                invalidSymbols(readkey,"key");
                invalidSymbols(readValue,"value");


            }
            

            if (readkey != NULL && readSection != NULL)
                if (strcmp(readSection,section) ==0 && strcmp(readkey,key) ==0)
                    printf("%s %s %s\n",readSection,readkey,readValue);
        }    
    }
    else
        printf("file open unsuccessful");
    
    printf("hello");
}
