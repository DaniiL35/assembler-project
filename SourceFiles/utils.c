#include "utils.h"




/* Hash function */
unsigned int hash(char *str) {
    unsigned int hash = 5381;
    int c;
    unsigned int index;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    index = hash % TABLE_SIZE;
    return index;
}
/* Function to combine two strings with malloc */
char *strcatWithMalloc(const char *str1, const char *str2) {
    int totalLength = strlen(str1) + strlen(str2) + 1;
    char *fullName = (char *)malloc(totalLength);
    if (fullName == NULL) {
        exit(EXIT_FAILURE);
    }
    strcpy(fullName, str1);
    strcat(fullName, str2);
    return fullName;
}


/* Function to open a file and check if it was opened successfully */
FILE *openFileAndCheck(const char *filePath, const char *mode) {
    FILE *file = fopen(filePath, mode);
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }
    return file;
}