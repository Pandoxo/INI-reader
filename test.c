#include <stdio.h>
#include<stdlib.h>
#include <string.h>



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
    
    strcpy(path,argv[1]);
    fptr = fopen(path,"r");
    

    char* tokPtr;
    tokPtr = strtok(argv[2],".");
    char* section = tokPtr;
    tokPtr = strtok(NULL,",");
    char* key= tokPtr;
    printf("%s\n",section);
    printf("%s\n",key);
    printf("%s\n\n",path);
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
                char* tokSectionPtr = strtok(content,"[]");
                readSection = strdup(tokSectionPtr);
                ifmissing(readSection,"section");
                invalidSymbols(readSection,"section");
            }else if (firstLetter == '\n')
            {
                continue;
            }
            else
            {
                //Reading key and value
                char* tokPtr2;
                tokPtr2 = strtok(content," ");
                readkey = strdup(tokPtr2);
                tokPtr2 = strtok(NULL," "); // "="
                tokPtr2 = strtok(NULL," \n");
                readValue = strdup(tokPtr2);
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
