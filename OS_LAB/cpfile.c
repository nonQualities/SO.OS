
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// the is program is supposed to copy a file as it is in binary program
// it starts with a simlple error check!

int main(int argc, char **argv) {
  // expects three arguments:
  // - source file
  // - copy source
  // - destination source
  //
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SOURCE DESTINATION\n", argv[0]);
        return EXIT_FAILURE;
    }

    // opens a file in binary from the second arg
    FILE *in = fopen(argv[1], "rb");  // "rb" means read in binary mode
    if(in == NULL){
        perror(argv[2]);
        fclose(in);
        return EXIT_FAILURE;
    }


    unsigned char buffer[8192];
    size_t n;
    int failed = 0;

    // on success fread() returns the number of bytes read.
    // So until it keeps reading n has a positive value, and when it finishes it
    // shortcircuits to false i.e. 0

    while ((n = fread(buffer, 1, sizeof(buffer), in))) {
        // fwrite() writes the read input stream into the stdout (terminal)
        if(fwrite(buffer, 1, n, stdout) != n){
            perror("fwrite");
            failed = 1; break;
        }
    }

    /*
     * * In C, a file stream (FILE *) maintains certain internal status
     indicators. Two important indicators are:
          1. EOF indicator — tells you whether an end-of-file condition has occurred
          2. Error indicator — tells you whether an I/O operation has encountered an error.

       if an I/O error occurs during this operation, the stream's error
     indicator is set. fread() checks if the error is set to stream, and returns
     0 if not or a non-zero value otherwise.
    */

    if (ferror(in)) { perror("fread"); failed = 1;}
    if(fclose(in) != 0){ perror("fclose input"); failed = 1; }
    if(fclose(stdout) != 0){ perror("fclose output"); failed = 1;}

    
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
