#include <stdio.h>
#include<stdlib.h>
#include <string.h>


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
        char* readSection;
        while (fgets(content,63,fptr))
        {
            char firstLetter = content[0];    
            char* readkey;
            char* readValue;
            if (firstLetter == '[')
            {
                char* tokSectionPtr = strtok(content,"[]");
                readSection = strdup(tokSectionPtr);
            }else
            {
                char* tokPtr2;
                tokPtr2 = strtok(content," ");
                readkey = strdup(tokPtr2);
                tokPtr2 = strtok(NULL," "); // "="
                tokPtr2 = strtok(NULL," ");
                readValue = strdup(tokPtr2);
            
            if (readkey != NULL && readSection != NULL)
                if (strcmp(readSection,section) ==0 && strcmp(readkey,key) ==0)
                    printf("%s %s %s\n",readSection,readkey,readValue);
            }
       }    
    }
    else
        printf("file open unsuccessful");
    
    printf("hello");
}
