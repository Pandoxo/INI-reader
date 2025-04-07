#include <stdio.h>
#include<stdlib.h>
#include <string.h>


struct Section;
struct Array;


typedef struct {
    char *key;
    char *value;
} KeyValuePair;



typedef struct Section{
    char *name;
    KeyValuePair* pairs;
    int numberOfPairs;
    int sizeOfPairs;
} Section;



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

int addSection(Section** a,int i, int capacity, Section* element)
{
    if(i>=capacity)
    {
        capacity *=2;
        Section* b  = realloc(*a,capacity*sizeof(Section));
        if(!b)
        {
            perror("realloc");
            exit(1);
        }
    }
    (*a)[i] = *element;
    return capacity;
}
int addPair(KeyValuePair** a,int i, int capacity, KeyValuePair* element)
{
    if(i>=capacity)
    {
        capacity *=2;
        KeyValuePair* b  = realloc(*a,capacity*sizeof(KeyValuePair));
        if(!b)
        {
            perror("realloc");
            exit(1);
        }
    }
    (*a)[i] = *element;
    return capacity;
}


int main(int argc, char* argv[])
{
    FILE *fptr;
    char content[63];
    char path[20];

    //Initialize dynamic array
    int size_sections = 20;
    int i = 0; //numer of section
    Section* sections = malloc(size_sections*sizeof(Section));

    

    
    strcpy(path,argv[1]);
    fptr = fopen(path,"r");
    
    
    // char* tokPtr;
    // tokPtr = strtok(argv[2],".");
    // char* section = tokPtr;
    // tokPtr = strtok(NULL,",");
    // char* key= tokPtr;
    // printf("%s\n",section);
    // printf("%s\n",key);
    // printf("%s\n\n",path);
    if (fptr != NULL)
    {
        printf("\nfile opened\n");
        while (fgets(content,63,fptr))
        {
            char firstLetter = content[0];   
            printf("%s",content); 
            if (firstLetter == '[')
            {
                //Reading section
               
                char* tokSectionPtr = strtok(content,"[]");

                Section sec;
                sec.name = strdup(tokSectionPtr);
                sec.numberOfPairs = 0;
                sec.sizeOfPairs = 5;

                ifmissing(sec.name,"section");
                invalidSymbols(sec.name,"section");
                size_sections = addSection(&sections,i,size_sections,&sec);
                

                //Initialize KeyValuePairs array
                KeyValuePair* pairsArray = malloc(sec.sizeOfPairs*sizeof(KeyValuePair));
                sections[i].pairs = pairsArray;

                

            }else if (firstLetter == '\n')
            {
                //Empty line
                i++;
                continue;
            }
            else
            {
                KeyValuePair pair;

                //Reading key and value
                char* tokPtr2;
                
                tokPtr2 = strtok(content," ");

                pair.key = strdup(tokPtr2);
                tokPtr2 = strtok(NULL," "); // "="
                tokPtr2 = strtok(NULL," \n");
                pair.value = strdup(tokPtr2);
                
                ifmissing(pair.key,"key");
                ifmissing(pair.value,"value");
                invalidSymbols(pair.key,"key");
                invalidSymbols(pair.value,"value");
                
                sections[i].sizeOfPairs = addPair(&sections[i].pairs, sections[i].numberOfPairs, sections[i].sizeOfPairs, &pair);
                sections[i].numberOfPairs++;                
            }     
            // if (readkey != NULL && readSection != NULL)
            //     if (strcmp(readSection,section) ==0 && strcmp(readkey,key) ==0)
            //         printf("%s %s %s\n",readSection,readkey,readValue);
            // if (sections[0].numberOfPairs == 4)
            // {
            //     for (int j = 0;j <4;j++)
            //     {
            //         printf("key: %s value: %s\n",sections[0].pairs[j].key,sections[0].pairs[j].value);
            //     }
            //     exit(0);
            // }
        }    

    }
    else
        printf("file open unsuccessful");
    
    printf("hello");
}
