#include "assembler.h"


/* Function to check if the file exists */
int checkFile(char *fName) {
    char *fullFileName;
    FILE *file;

    fullFileName = strcatWithMalloc(fName, ".as");  

    file = fopen(fullFileName, "r");
    if (file == NULL) {
        fprintf(stdout, "Error: File \"%s\" does not exist\n", fullFileName);
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
    char *validatedFileName;
    struct labelTable *labelTable;

    if (argc < 2) {
        fprintf(stdout, "Usage: %s <filename1> <filename2> ...\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        char *fName = argv[i];
        if (checkFile(fName)) {
            if (strcmp(preprocessor(fName), "error") == 0) {
                continue;
            }

            validatedFileName = validation(fName);
            if (strcmp(validatedFileName, "error") != 0) {
                labelTable = firstpass(validatedFileName, fName);
                secondpass(validatedFileName, labelTable, fName);
                free(validatedFileName);
            }
        }
    }

    return 0;
}

