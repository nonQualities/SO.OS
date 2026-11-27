/*
 * Experiment 3.2: Simulation of the UNIX 'grep' Command
 * What it does: Searches a text file line-by-line for a specified substring
 *               and prints every matching line along with its filename and
 *               1-based line number.
 * How it works: Opens the file in read mode via fopen(), reads bounded lines
 *               with fgets(), strips trailing newlines via strcspn(), checks
 *               substring membership with strstr(), and closes the stream.
 * Related OS Concepts:
 *   - Buffered Text File I/O (fopen, fgets, fclose)
 *   - Line-Oriented Text Stream Processing in UNIX Filter Utilities
 *   - Substring Pattern Matching and Defensive Fixed-Buffer Line Reading
 */

#include <stdio.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[])
{
    char line[MAX_LINE];
    FILE *fp;
    int line_no = 0;

    /* Expect two CLI arguments: <file> and <word>. */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <word>\n", argv[0]);
        return 1;
    }

    /* Open the target file in read-only text mode. */
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror(argv[1]);
        return 1;
    }

    /* Read the file sequentially line-by-line up to MAX_LINE - 1 bytes. */
    while (fgets(line, sizeof line, fp) != NULL) {
        line_no++;
        line[strcspn(line, "\n")] = '\0';   /* remove trailing newline */

        /* Print formatted match if the search word occurs anywhere in the line. */
        if (strstr(line, argv[2]) != NULL)
            printf("%s:%d: %s\n", argv[1], line_no, line);
    }

    fclose(fp);
    return 0;
}
