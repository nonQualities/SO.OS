#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main( int argc, char **argv){
    const char* PATH  = (argc > 1) ? argv[1] : ".";
    DIR *dir  = opendir(PATH);

    if(dir == NULL){
        perror("opendir");
        return EXIT_FAILURE;
    }

    errno  = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        printf("%s\n", entry->d_name);
    }

    if(errno != 0){
        perror("readdir");
        closedir(dir);
        return EXIT_FAILURE;
    }

    if (closedir(dir)== -1){
        perror("closedir");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}