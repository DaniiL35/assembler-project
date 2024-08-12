#include "utils.h"
#include "preprocessor.h"
#include "validation.h"


/* Function to check if the file exists */
int checkFile(char *fName) {
    char *fullFileName;
    FILE *file;

    fullFileName = strcatWithMalloc(fName, ".as");  

    file = fopen(fullFileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: File \"%s\" does not exist\n", fullFileName);
        free(fullFileName);  
        return 0;
    }

    fclose(file);
    free(fullFileName);  
    return 1;
}

/* Main function */
int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename1> <filename2> ...\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        char *fName = argv[i];
        if (checkFile(fName)) {
            preprocessor(fName);
            validation(fName);
        }
    }

    return 0;
}
