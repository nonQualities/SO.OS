/*
 * File: dirSyscl.c
 * What it does: Prompts the user interactively for a directory path and displays all entries
 *               contained within that directory.
 * How it works: Reads a directory name from standard input into a fixed buffer, opens the
 *               directory stream with `opendir()`, reads each `struct dirent` entry sequentially
 *               via `readdir()`, and closes the stream with `closedir()`.
 * Related OS Concepts:
 *   - POSIX Directory Handling (`opendir`, `readdir`, `closedir`)
 *   - Directory Entries (`struct dirent` and `d_name`)
 *   - User-Space Wrapper Functions over Kernel Directory System Calls
 */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

/* Global pointer to hold each directory entry returned by readdir(). */
struct dirent *dptr;

int main(int argc, char *argv[]){
    char buff[100];
    DIR *drip;

    /* Prompt for and read the target directory path from standard input. */
    printf("\n\n ENTER DIRECTOR NAME");
    scanf("%s", buff);

    /* Attempt to open the directory stream; exit early if the path cannot be opened. */
    if((drip=opendir(buff)) == NULL){
        printf("The Given directory does not exist");
            printf("The Given Directory Doesn't exist");
            exit(EXIT_SUCCESS);
    }

    /* Iterate over all entries in the directory until readdir() returns NULL. */
    while((dptr = readdir(drip))){
        printf("%s\n", dptr->d_name);
    }

    /* Release the open directory stream and associated OS resources. */
    closedir(drip);

    return 0;
}
