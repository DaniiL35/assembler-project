#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>  
#include "preprocessor.h"
#include "validation.h"


struct instraction {
    char *name;
    int opcode;
    const char *source;
    const char *dest;
};

/* The instruction table */
struct instraction instraction_table[16] = {
    {"mov", 0, "0123", "0123"},
    {"cmp", 1, "0123", "0123"},
    {"add", 2, "0123", "0123"},
    {"sub", 3, "0123", "0123"},
    {"lea", 4, "123", "1"},
    {"clr", 5, "123", ""},
    {"not", 6, "123", ""},
    {"inc", 7, "123", ""},
    {"dec", 8, "123", ""},
    {"jmp", 9, "12", ""},
    {"bne", 10, "12", ""},
    {"red", 11, "123", ""},
    {"prn", 12, "0123", ""},
    {"jsr", 13, "12", ""},
    {"rts", 14, "source", ""},
    {"stop", 15, "source", ""}
};

/* Checks if the word is a directive.
   Returns 1 if it is a directive, 0 otherwise. */
int is_directive(char *word) {
    if (strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0 || strcmp(word, ".entry") == 0 || strcmp(word, ".extern") == 0) {
        return 1;
    }
    return 0;
}

/* Checks if the line is empty (only whitespace).
   Returns 1 if the line is empty, 0 otherwise. */
int is_empty(char *line) {
    char *ptr = line;
    while (*ptr) {
        if (!isspace((unsigned char)*ptr)) {
            return 0;
        }
        ptr++;
    }
    return 1;
}

/* Checks if the word is an instruction.
   Returns 1 if it is an instruction, 0 otherwise. */
int is_instruction(char *word) {
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(word, instraction_table[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Checks if the first word of the line is a label.
   Returns 1 if it is a label, 0 otherwise. */
static int isLabel(char *line) {
    char label[MAX_LABEL_LENGTH];
    sscanf(line, "%s", label);

    if (label[strlen(label) - 1] == ':') {
        return 1;
    }
    return 0;
}

/* need to implement */
static int is_number(char *str, int max, int min, int *result) {
    return 0; /* need to implement */
}



/* remove char from a word (f for first, l for last )*/
void remove_by_place(char *str, char place) {
    size_t len = strlen(str);
    if (len > 0) {
        if (place == 'f') {
            memmove(str, str + 1, len);
        }
        if (place == 'l') {
            str[len - 1] = '\0';
        }
    }
}

char *validation(char *fName) {
    char Current_Line[Max_LINE_LEN];
    char *am_file_name;
    int i = 0;
    FILE *am_file;
    am_file_name = strcatWithMalloc(fName, am_file_ext);
    am_file = openFileAndCheck(am_file_name, "w+");
    /* reading line by line from the as file */
    while (fgets(Current_Line, Max_LINE_LEN, am_file) != 0) { 
        




    }
    return am_file_name;

}


int main() {
    char *fName = "test";
    validation(fName);
    return 0;
}
