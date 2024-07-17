#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "preprocessor.h"
#include "validation.h"

struct instruction {
    char *name;
    int opcode;
    const char *source;
    const char *dest;
    const int num_of_operands;
};

/* The instruction table */
struct instruction instruction_Table[INSTRUCTION_TABLE_SIZE] = {
    {"mov", 0, "0123", "0123", 2},
    {"cmp", 1, "0123", "0123", 2},
    {"add", 2, "0123", "0123", 2},
    {"sub", 3, "0123", "0123", 2},
    {"lea", 4, "123", "1", 2},
    {"clr", 5, "123", "", 1},
    {"not", 6, "123", "", 1},
    {"inc", 7, "123", "", 1},
    {"dec", 8, "123", "", 1},
    {"jmp", 9, "12", "", 1},
    {"bne", 10, "12", "", 1},
    {"red", 11, "123", "", 1},
    {"prn", 12, "0123", "", 1},
    {"jsr", 13, "12", "", 1},
    {"rts", 14, "", "", 0},
    {"stop", 15, "", "", 0}
};

char * addressing_method(char *str) {
    if (str[0] == '#') {
        return "0";
    }
    /* if (Direct address )
        return 1; */
    if (str[0] == '*') {
        if (valid_register(str)) 
            return "2";
            }
    if (valid_register(str)) {
    return "3";
    }
    printf("Error: invalid addressing method\n");
    return NULL;
}

int valid_register(char *str) {
    if (str[0] == 'r' && str[1] >= '0' && str[1] <= '7') {
        return true;
    }
    return false;
}


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
        if (strcmp(word, instruction_Table[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_alphanumeric_string(const char *str) {
    while (*str) {
        if (!isalnum(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

/* Checks if the first word of the line is a label.
   Returns 1 if it is a label, 0 otherwise. */
static int isLabel(char *word) {
    char label[MAX_LABEL_LENGTH];
    sscanf(word, "%s", label);

    if (label[strlen(label) - 1] == ':') {
        return true;
    }
    return false;
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
    char first_word[Max_LINE_LEN], second_word[Max_LINE_LEN];
    char *method; 
    char *w2; 
    char *am_file_name;
    int line_counter = 0;
    FILE *am_file;
    am_file_name = strcatWithMalloc(fName, am_file_ext);
    am_file = openFileAndCheck(am_file_name, "w+");

    /* reading line by line from the as file */
    while (fgets(Current_Line, Max_LINE_LEN, am_file) != 0) {
        line_counter++;
        /* Check if the line is empty (only whitespace) */
        if (is_empty(Current_Line))
            continue;

        /* checks for command line */
        if (Current_Line[0] == ';')
            fputs(Current_Line, am_file);
            continue;

        /* checks if the first word is label */
        sscanf(Current_Line, " %s %s ", first_word, second_word);
        if (isLabel(first_word)) {
            /* next word must be instruction */
            w2 = strpbrk(Current_Line, " ");
            w2++;
            if (is_instruction(w2)) {
                w2 = strpbrk(w2, " ");
                w2++;
                method = addressing_method(w2);
                printf("method: %s\n", method);



            }
        }
    }
    return NULL; 
}

int main() {
    char *fName = "test";
    validation(fName);
    return 0;
}
