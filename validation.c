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

/* find in the instruction table */
int find_in_table(char *word) {
    int i;
    for (i = 0; i < INSTRUCTION_TABLE_SIZE; i++) {
        if (strcmp(word, instruction_Table[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

char *addressing_method(char *str) {
    if (str[0] == '#') {
        return "0";
    }
    /* if (Direct address )
        return 1; */
    if (str[0] == '*') {
        if (valid_register(str)) {
            return "2";
        }
    }
    if (valid_register(str)) {
        return "3";
    }
    printf("Error: invalid addressing method\n");
    return NULL;
}

int valid_register(char *str) {
    if (str[0] == 'r' && str[1] >= '0' && str[1] <= '7' && (str[3] == ' ' || str[3] == ',')) {
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
int is_empty(const char *line) {
    while (*line != '\0') {
        if (!isspace(*line)) {
            return 0; 
        }
        line++;
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

/* skipping functions */
void skip_to_next_word(char **str) {
    char *next_space = strpbrk(*str, " ");
    if (next_space != NULL) {
        *str = next_space + 1;
    }
}

void skip_to_the_next_operand(char **str) {
    char *next_comma = strpbrk(*str, ",");
    if (next_comma != NULL) {
        next_comma++;
        while (*next_comma == ' ') {
            next_comma++;
        }
        *str = next_comma;
    }
}

char *validation(char *fName) {
    char Current_Line[Max_LINE_LEN];
    char *addres_mode;
    char *current_word;
    char *am_file_name;
    int line_counter = 0;
    FILE *am_file;
    am_file_name = strcatWithMalloc(fName, am_file_ext);
    am_file = openFileAndCheck(am_file_name, "r");
    current_word = (char *)malloc(Max_LINE_LEN * sizeof(char));

    while (fgets(Current_Line, Max_LINE_LEN, am_file) != 0) {
        printf(" reading started \n"); /* testing only */
        printf("Current line: %s\n", Current_Line); /* testing only */
        line_counter++;
        /* skip empty lines */
        if (is_empty(Current_Line)){
            printf("Empty line found\n"); /* testing only */
            continue;}
        /* skip comments */
        if (Current_Line[0] == ';') {
            printf("Comment found %s\n", Current_Line); /* testing only */
            continue;
        }
        /* for rows with label */
        sscanf(Current_Line, " %s", current_word);
        if (isLabel(current_word)) {
            skip_to_next_word(&current_word);
            if (is_instruction(current_word)) {
                skip_to_next_word(&current_word);
                addres_mode = addressing_method(current_word);
                if (addres_mode == NULL) {
                    printf("Error: invalid addressing method\n");
                    free(current_word);
                    return NULL;
                }
                if (strstr(addres_mode, instruction_Table[find_in_table(current_word)].source) == NULL) {
                    printf("Error: invalid source operand\n");
                    free(current_word);
                    return NULL;
                }
                skip_to_the_next_operand(&current_word);
                addres_mode = addressing_method(current_word);
                if (strstr(addres_mode, instruction_Table[find_in_table(current_word)].dest) == NULL) {
                    printf("Error: invalid destination operand\n");
                    free(current_word);
                    return NULL;
                }
            }
        }
        /* for rows without label*/
        else {
            if (is_instruction(current_word)) {
                skip_to_next_word(&current_word);
                addres_mode = addressing_method(current_word);
                if (addres_mode == NULL) {
                    printf("Error: invalid addressing method\n");
                    free(current_word);
                    return NULL;
                }
                if (strstr(addres_mode, instruction_Table[find_in_table(current_word)].source) == NULL) {
                    printf("Error: invalid source operand\n");
                    free(current_word);
                    return NULL;
                }
                skip_to_the_next_operand(&current_word);
                addres_mode = addressing_method(current_word);
                if (strstr(addres_mode, instruction_Table[find_in_table(current_word)].dest) == NULL) {
                    printf("Error: invalid destination operand\n");
                    free(current_word);
                    return NULL;
                }
            }
        }
    }
    fclose(am_file);
    free(current_word);
    return NULL;
}

int main() {
    char *fName = "test";
    (validation(fName));
    return 0;
}
