/*
 * Experiment 2.1: Directory Listing Using System Calls (Interactive Input)
 * What it does: Prompts the user for a directory path and lists all entries
 *               (files, subdirectories, '.' and '..') contained within it.
 * How it works: Reads a bounded path string from stdin, opens a POSIX
 *               directory stream via opendir(), iterates through each
 *               struct dirent entry using readdir(), and closes it with
 *               closedir().
 * Related OS Concepts:
 *   - POSIX Directory Streams (DIR *) and struct dirent
 *   - Directory Traversal and Underlying getdents() Kernel System Call
 *   - Error Diagnostics via perror() and Standard Error Stream (stderr)
 */

#include <stdio.h>
#include <dirent.h>

int main(void)
{
    char path[256];
    struct dirent *entry;
    DIR *dir;

    /* Read up to 255 characters to prevent buffer overflow on path input. */
    printf("Enter directory name: ");
    if (scanf("%255s", path) != 1) {
        fprintf(stderr, "No directory name given.\n");
        return 1;
    }

    /* Open the directory stream; returns NULL if path is invalid/inaccessible. */
    dir = opendir(path);
    if (dir == NULL) {
        perror(path);
        return 1;
    }

    /* Sequentially fetch each directory entry until end-of-stream (NULL). */
    while ((entry = readdir(dir)) != NULL)
        printf("%s\n", entry->d_name);

    /* Release the directory stream and its underlying file descriptor. */
    closedir(dir);
    return 0;
}
