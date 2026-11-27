/*
 * Experiment 3.3: Pattern Matching and Occurrence Counting in a File
 * What it does: Scans a file for a non-empty pattern string, displays each
 *               matching line with its line number, and reports the total
 *               number of non-overlapping occurrences across the file.
 * How it works: Reads lines sequentially using fgets(), repeatedly advances a
 *               scan pointer via strstr() and strlen(pattern) to count all
 *               non-overlapping matches per line, and accumulates the total.
 * Related OS Concepts:
 *   - Sequential File Access and Buffered Stream Processing
 *   - Non-Overlapping Substring Scanning in Text Utilities
 *   - Input Validation (Empty Pattern Guard) and Resource Cleanup (fclose)
 */

#include <stdio.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[])
{
    char line[MAX_LINE];
    const char *pattern;
    FILE *fp;
    int line_no = 0;
    int occurrences = 0;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <pattern>\n", argv[0]);
        return 1;
    }

    /* Reject an empty pattern to prevent an infinite zero-advance loop. */
    pattern = argv[2];
    if (*pattern == '\0') {
        fprintf(stderr, "Pattern cannot be empty.\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror(argv[1]);
        return 1;
    }

    /* Process each line and find all non-overlapping occurrences within it. */
    while (fgets(line, sizeof line, fp) != NULL) {
        char *match = line;
        int line_occurrences = 0;

        line_no++;
        while ((match = strstr(match, pattern)) != NULL) {
            line_occurrences++;
            occurrences++;
            match += strlen(pattern);   /* advance past matched pattern */
        }

        /* Print the line only once even if the pattern appeared multiple times. */
        if (line_occurrences > 0) {
            line[strcspn(line, "\n")] = '\0';
            printf("%s:%d: %s\n", argv[1], line_no, line);
        }
    }

    fclose(fp);
    printf("Total occurrences of \"%s\": %d\n", pattern, occurrences);

    return 0;
}
