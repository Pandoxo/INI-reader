#include <stdio.h>
#include<stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
    FILE *fptr;
    fptr = fopen("example-3.0.ini.txt","r");
    char content[63];

    char path[20];
    strcpy(path,argv[1]);


    char* tokPtr;
    tokPtr = strtok(argv[2],".");
    char* section = tokPtr;
    tokPtr = strtok(NULL,",");
    char* key= tokPtr;
    free(tokPtr);
    printf("%s\n",section);
    printf("%s\n",key);
    printf("%s\n\n",path);
    if (fptr != NULL)
    {
       while (fgets(content,63,fptr))
       {
            char firstLetter = content[0];

            
            char readSection[30];
            char readkey[30];
            char readValue[30];
            if (firstLetter == '[')
            {
                int i = 0;
                memset(readSection,0,30);

                while (content[i+1] !=']')
                {
                    readSection[i] = content[i+1];
                    i++;
                }
            }else
            {
                char* tokPtr;
                tokPtr = strtok(content," ");
                char* readkey = tokPtr;
                tokPtr = strtok(NULL," ");
                tokPtr = strtok(NULL," ");
                readValue = *tokPtr

            }   
            if (strcmp(readSection,section) ==0)

                    printf("%s %s\n",readSection,readkey);
       }    
    }
    else
        printf("file open unsuccessful");
    
    printf("hello");
}
