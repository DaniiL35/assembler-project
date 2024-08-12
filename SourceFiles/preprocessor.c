#include "preprocessor.h"


/* Macro structure */
struct Macro {
    char mName[Max_macro_len_name];
    char **lines;
    int lineC;
    struct Macro *next;
};


const char *invalid_macro_names[invalid_macro_table_size] = {
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "macro", "endmacr", "data", "string", "entry", "extern"
};

/* Macro table structure */
struct MacroTable {
    struct Macro *table[TABLE_SIZE];
};



/* Initialize the hash table */
void initMacroTable(struct MacroTable *mTable) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        mTable->table[i] = NULL;
    }
}

/* Function to free the macro table */
void freeMacroTable(struct MacroTable *mTable) {
    int i, j;
    for ( i = 0; i < TABLE_SIZE; i++) {
        struct Macro *macro = mTable->table[i];
        while (macro != NULL) {
            struct Macro *temp = macro;
            macro = macro->next;
            for ( j = 0; j < MAX_LINE_LEN; j++) {
                free(temp->lines[j]);
            }
            free(temp->lines);
            free(temp);
        }
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

int macro_line(char *line_buffer, struct Macro **currentMacro, struct MacroTable *mTable) {
    char *p1 = line_buffer, firstWord[MAX_LINE_LEN], macro_name[MAX_LINE_LEN];
    struct Macro *newMacro = NULL;
    int i = 0;
    int j;
    sscanf(p1, " %s %s", firstWord, macro_name); /* get the first word in the line */
    
    /* macro def */
    if (strcmp(firstWord, Macro_start) == 0) {
        newMacro = (struct Macro *)malloc(sizeof(struct Macro));
        if (newMacro == NULL) {
            exit(EXIT_FAILURE);
        }
        
        /* Check if the macro name starts with a digit */
        if (isdigit(macro_name[0])) {
            printf("Error: Macro name '%s' cannot start with a digit.\n", macro_name);
            free(newMacro);
            return -1;
        }
        
        /* Check if the macro name starts with an invalid name */
        for (i = 0; i < invalid_macro_table_size; i++) {
            if (strncmp(macro_name, invalid_macro_names[i], strlen(invalid_macro_names[i])) == 0) {
                printf("Error: Macro name '%s' cannot start with '%s'.\n", macro_name, invalid_macro_names[i]);
                free(newMacro);
                return -1;
            }
        }
        
        /* check if the macro already exists */
        if (search_macro(mTable, macro_name) != NULL) { 
            free(newMacro);
            return -1;
        }
        
        /* create a new macro */
        strcpy(newMacro->mName, macro_name);
        newMacro->lineC = 0;
        newMacro->lines = (char **)malloc(MAX_LINE_LEN * sizeof(char *));
        if (newMacro->lines == NULL) {
            free(newMacro);
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < MAX_LINE_LEN; i++) {
            newMacro->lines[i] = (char *)malloc(MAX_LINE_LEN * sizeof(char));
            if (newMacro->lines[i] == NULL) {
                for (j = 0; j < i; j++) {
                    free(newMacro->lines[j]);
                }
                free(newMacro->lines);
                free(newMacro);
                exit(EXIT_FAILURE);
            }
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
    char Current_Line[MAX_LINE_LEN];
    char *as_file_name;
    char *am_file_name;
    int i = 0;
    FILE *as_file;
    FILE *am_file;
    struct Macro *currentMacro = NULL;
    struct MacroTable mTable;
    initMacroTable(&mTable);
    as_file_name = strcatWithMalloc(fName, AS_FILE_EXT);
    am_file_name = strcatWithMalloc(fName, AM_FILE_EXT);
    as_file = openFileAndCheck(as_file_name, "r");
    am_file = openFileAndCheck(am_file_name, "w");
    /* reading line by line from the as file */
    while (fgets(Current_Line, MAX_LINE_LEN, as_file) != 0) { 
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
    free(am_file_name);
    freeMacroTable(&mTable);

    return am_file_name;

}

