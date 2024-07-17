#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "preprocessor.h"

/* Macro structure */
struct Macro {
    char mName[Max_macro_len_name];
    char **lines;
    int lineC;
    struct Macro *next;
};

/* Hash table structure */
struct MacroTable {
    struct Macro *table[TABLE_SIZE];
};

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

/* Initialize the hash table */
void initMacroTable(struct MacroTable *mTable) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        mTable->table[i] = NULL;
    }
}

/* Insert a macro into the hash table */
void insertMacro(struct MacroTable *mTable, struct Macro *macro) {
    unsigned int index = hash(macro->mName);
    macro->next = mTable->table[index];
    mTable->table[index] = macro;
}

/* Find a macro in the hash table */
struct Macro *search_macro(struct MacroTable *mTable, char *name) {
    unsigned int index = hash(name);
    struct Macro *macro = mTable->table[index];
    while (macro != NULL && strcmp(macro->mName, name) != 0) {
        macro = macro->next;
    }
    return macro;
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


/* Function to check the type of the line */
int macro_line(char *line_buffer, struct Macro **currentMacro, struct MacroTable *mTable) {
    char *p1 = line_buffer, firstWord[Max_LINE_LEN], macro_name[Max_LINE_LEN];
    struct Macro *newMacro = (struct Macro *)malloc(sizeof(struct Macro));
    int i = 0;
    sscanf(p1, " %s %s", firstWord, macro_name); /* get the first word in the line */
    
    /* macro def */
    if (strcmp(firstWord, Macro_start) == 0) {
        /* check if the macro already exists */
        if (search_macro(mTable, macro_name) != NULL) { 
            return -1;
        }
        /* create a new macro */
        strcpy(newMacro->mName, macro_name);
        newMacro->lineC = 0;
        newMacro->lines = (char **)malloc(Max_LINE_LEN * sizeof(char *));
        for (i = 0; i < Max_LINE_LEN; i++) {
            newMacro->lines[i] = (char *)malloc(Max_LINE_LEN * sizeof(char));
        }
        insertMacro(mTable, newMacro);
        *currentMacro = newMacro;
        return 0;
    }

    /* end of macro def */
    if (strcmp(firstWord, Macro_end) == 0) {
        *currentMacro = NULL;
        return 1;
    }

    /* macro call */
    if (search_macro(mTable, firstWord) != NULL) {
        *currentMacro = search_macro(mTable, firstWord);
        return 2;
    }

    /* default value for any other line */
    return 3;
}


/* Function to preprocess the as file */
char *preprocessor(char *fName) {
    char Current_Line[Max_LINE_LEN];
    char *as_file_name;
    char *am_file_name;
    int i = 0;
    FILE *as_file;
    FILE *am_file;
    struct Macro *currentMacro = NULL;
    struct MacroTable mTable;
    initMacroTable(&mTable);
    as_file_name = strcatWithMalloc(fName, as_file_ext);
    am_file_name = strcatWithMalloc(fName, am_file_ext);
    as_file = openFileAndCheck(as_file_name, "r");
    am_file = openFileAndCheck(am_file_name, "w");
    /* reading line by line from the as file */
    while (fgets(Current_Line, Max_LINE_LEN, as_file) != 0) { 
        switch (macro_line(Current_Line, &currentMacro, &mTable)) { 
            case 0: /* start of macro def */
                /* logic needs to be implemented */
                break;
            case 1: /* end of macro def */
                currentMacro = NULL;
                break;
            case 2: /* macro call (replace the macro name with data) */
                for (i = 0; i < currentMacro->lineC; i++) {
                    fputs(currentMacro->lines[i], am_file);
                }
                currentMacro = NULL;
                break;
            case 3: /* normal line */
                if (currentMacro != NULL) { /* if we are in a macro def */
                    strcpy(currentMacro->lines[currentMacro->lineC++], Current_Line); /* add the line to the macro */
                } else {
                    fputs(Current_Line, am_file); /* write the line to the am file without changing anything */
                }
                break;
        }
    }

    fclose(as_file);
    fclose(am_file);
    free(as_file_name);
    return am_file_name;
}


/* test main function 
int main() {
    char *fName = "test";
    preprocessor(fName);
    return 0;
}
*/

