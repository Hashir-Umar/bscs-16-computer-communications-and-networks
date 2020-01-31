#include <stdio.h>
#include <string.h>

int main() {

    char arr[100] = "folder file1.txt,file2.txt,file3.txt";

    char spaceDelimeter = ' ';
    char commaDelimeter = ',';

    char *folder = strtok(arr, &spaceDelimeter);
    printf("Folder: %s", folder);
    
    int i = 0;
    while(++i) {
        char *myFile = strtok(NULL, &commaDelimeter);
        if(myFile == NULL) 
            break;
            
        printf("\nFile %d: %s", i, myFile);
    }
    

    printf("\n");
    return 0;
}   
