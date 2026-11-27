
/*
 * File: cpfile.c
 * What it does: Reads a source file in binary mode and writes its raw byte stream to standard output
 *               (a minimal binary stream-copy utility).
 * How it works: Validates CLI argument count, opens the source file via buffered C stdio (`fopen`
 *               with "rb"), repeatedly transfers 8 KiB chunks with `fread` and `fwrite`, checks
 *               stream error state with `ferror`, and closes both streams.
 * Related OS Concepts:
 *   - Buffered User-Space I/O (`FILE *` streams) vs. Unbuffered System Calls (`read`/`write`)
 *   - File Descriptors, Standard Streams (`stdin`, `stdout`, `stderr`), and Error Indicators
 *   - Chunked Buffer Transfers to Amortize Kernel System Call Overhead
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    /* Verify that two command-line arguments (SOURCE and DESTINATION) are supplied. */
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SOURCE DESTINATION\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Open the source file in binary read mode ("rb") to prevent newline translation. */
    FILE *in = fopen(argv[1], "rb");
    if(in == NULL){
        perror(argv[2]);
        fclose(in);
        return EXIT_FAILURE;
    }

    /* Allocate an 8 KiB user-space transfer buffer to minimize I/O call frequency. */
    unsigned char buffer[8192];
    size_t n;
    int failed = 0;

    /*
     * Buffered copy loop: fread() returns the number of bytes read (0 on EOF or error).
     * Each read chunk is immediately flushed to stdout via fwrite().
     */
    while ((n = fread(buffer, 1, sizeof(buffer), in))) {
        /* Write the exact number of bytes read (`n`) to stdout; detect short writes on error. */
        if(fwrite(buffer, 1, n, stdout) != n){
            perror("fwrite");
            failed = 1; break;
        }
    }

    /*
     * Distinguish normal End-Of-File (EOF) from an underlying read failure using ferror(),
     * since fread() returns 0 in both cases.
     */
    if (ferror(in)) { perror("fread"); failed = 1;}

    /* Release stream buffers and underlying OS file descriptors, reporting flush/close errors. */
    if(fclose(in) != 0){ perror("fclose input"); failed = 1; }
    if(fclose(stdout) != 0){ perror("fclose output"); failed = 1;}

    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
