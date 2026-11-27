/*
 * File: dir-list.c
 * What it does: Lists all entries (files, subdirectories, `.` and `..`) within a target
 *               directory, defaulting to the current working directory (`.`) if omitted.
 * How it works: Opens a directory stream using `opendir()`, iterates through directory entries
 *               with `readdir()` while monitoring `errno` to distinguish EOF from errors, and
 *               releases the directory stream with `closedir()`.
 * Related OS Concepts:
 *   - POSIX Directory Streams (`DIR *`) and Directory Entry Structures (`struct dirent`)
 *   - File System Hierarchy and Directory Traversal (`getdents` underlying system call)
 *   - POSIX Error Reporting via `errno` and `perror()`
 *
 * Logical Steps:
 *      - take args or use default and open a DIR *dir after handling error regarding userinput
 *      - use dirent struct dirent 
 */

#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main( int argc, char **argv){
    /* Use the user-provided path if supplied; otherwise default to current directory ("."). */
    const char* PATH  = (argc > 1) ? argv[1] : ".";
    DIR *dir  = opendir(PATH);

    /* Abort with diagnostic message if the directory cannot be opened (e.g., ENOENT/EACCES). */
    if(dir == NULL){
        perror("opendir");
        return EXIT_FAILURE;
    }

    /* Reset errno before readdir() so a NULL return at EOF can be distinguished from an error. */
    errno  = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        printf("%s\n", entry->d_name);
    }

    /* If errno became non-zero during iteration, an I/O or stream error occurred. */
    if(errno != 0){
        perror("readdir");
        closedir(dir);
        return EXIT_FAILURE;
    }

    /* Close the directory stream to release kernel file-descriptor resources. */
    if (closedir(dir)== -1){
        perror("closedir");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
