/*
 * Experiment 3.1: Simulation of the UNIX 'ls' Command
 * What it does: Lists all filenames and subdirectories inside a directory
 *               specified as a command-line argument.
 * How it works: Validates argc == 2, opens the target directory stream via
 *               opendir(argv[1]), prints each entry's d_name returned by
 *               readdir(), and closes the stream with closedir().
 * Related OS Concepts:
 *   - Command-Line Argument Passing (argc, argv) in UNIX Utilities
 *   - POSIX Directory Stream API (opendir, readdir, closedir)
 *   - File System Namespace and Directory Entry Metadata (struct dirent)
 */

#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    struct dirent *entry;
    DIR *dir;

    /* Enforce exact argument count: program name plus target directory. */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    /* Open directory stream and report OS-level errors (e.g., ENOENT) via perror. */
    dir = opendir(argv[1]);
    if (dir == NULL) {
        perror(argv[1]);
        return 1;
    }

    /* Traverse all directory entries until readdir() reaches end-of-directory. */
    printf("Contents of %s:\n", argv[1]);
    while ((entry = readdir(dir)) != NULL)
        printf("%s\n", entry->d_name);

    /* Close the stream to free kernel directory handle resources. */
    closedir(dir);
    return 0;
}
