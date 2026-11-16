#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

struct dirent *dptr;

int main(int argc, char *argv[]){
    char buff[100];
    DIR *drip;
    printf("\n\n ENTER DIRECTOR NAME");
    scanf("%s", buff);

    if((drip=opendir(buff)) == NULL){
        printf("The Given directory does not exist");
            printf("The Given Directory Doesn't exist");
            exit(EXIT_SUCCESS);
    }

    while((dptr = readdir(drip))){
        printf("%s\n", dptr->d_name);
    }

    closedir(drip);

    return 0;
}
