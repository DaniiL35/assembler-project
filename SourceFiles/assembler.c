#include "preprocessor.h"
#include "validation.h"
#include <stdio.h>

/* Main function */
int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename1> <filename2> ...\n", argv[0]);
        return 1;
    }

    for ( i = 1; i < argc; i++) {
        char *fName = argv[i];
        preprocessor(fName);
        validation(fName);
    }

    return 0;
}