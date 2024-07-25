#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "preprocessor.h"
#include "validation.h"

/* Structure to represent an instruction */
struct instruction {
    char *name;
    int opcode;
    const char *source;
    const char *dest;
    int num_of_operands;
};

/* Instruction table */
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

/* Function to find an instruction in the instruction table */
int find_in_table(char *word) {
    int i;
    for (i = 0; i < INSTRUCTION_TABLE_SIZE; i++) {
        if (strcmp(word, instruction_Table[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

/* Function to determine the addressing method of an operand */
char *addressing_method(char *str) {
    if (str[0] == '#') {
        return "0";
    }
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

/* Function to check if a string is a valid register */
int valid_register(char *str) {
    return (str[0] == 'r' && str[1] >= '0' && str[1] <= '7' && (str[2] == ' ' || str[2] == ',')) ? true : false;
}

/* Function to check if a word is a directive */
int is_directive(char *word) {
    return (strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0 || strcmp(word, ".entry") == 0 || strcmp(word, ".extern") == 0);
}

/* Function to check if a line is empty */
int is_empty(const char *line) {
    while (*line != '\0') {
        if (!isspace(*line)) {
            return 0; 
        }
        line++;
    }
    return 1; 
}

/* Function to check if a word is an instruction */
int is_instruction(char *word) {
    return find_in_table(word) != -1;
}

/* Function to check if a string is alphanumeric */
static int is_alphanumeric_string(const char *str, int length) {
    int i;
    for (i = 0; i < length; i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }
    return true;
}

/* Function to check if a word is a label */
static int is_label(char *word) {
    char label[MAX_LABEL_LENGTH];
    int len;
    sscanf(word, "%s", label);
    int len = strlen(label);

    if (label[len - 1] == ':') {
        return is_alphanumeric_string(label, len - 1);
    }
    return false;
}

/* Function to remove a character from a string */
void remove_by_place(char *str, char place) {
    size_t len = strlen(str);
    if (len > 0) {
        if (place == 'f') {
            memmove(str, str + 1, len);
        } else if (place == 'l') {
            str[len - 1] = '\0';
        }
    }
}

/* Function to skip to the next word in a string */
void skip_to_next_word(char **str) {
    char *next_space = strpbrk(*str, " ");
    if (next_space != NULL) {
        *str = next_space + 1;
    }
}

/* Function to skip to the next operand in a string */
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

/* Function to skip the leading sign in a string */
void skip_leading_sign(char **str) {
    if ((*str)[0] == '+' || (*str)[0] == '-') {
        (*str)++;
    }
}

/* Function to check if a string contains valid data */
int is_valid_data(char **str) {
    while (**str != '\0') {
        if (!isdigit(**str)) {
            return false;
        }
        strpbrk(*str, ",");
        skip_to_the_next_operand(str);
    }
    return true;
}

/* Function for validation */
char *validation(char *fName) {
    int line_counter = 1;
    char Current_Line[Max_LINE_LEN];
    char *addres_mode;
    char *current_word = (char *)malloc(Max_LINE_LEN * sizeof(char));
    char *am_file_name = strcatWithMalloc(fName, am_file_ext);
    FILE *am_file = openFileAndCheck(am_file_name, "r");
    FILE *temp_file = openFileAndCheck("temp.am", "w");

    while (fgets(Current_Line, Max_LINE_LEN, am_file) != 0) {
        printf("Reading started\n"); /* testing only */
        printf("Current line: %s\n", Current_Line); /* testing only */
        line_counter++;
        if (is_empty(Current_Line) || Current_Line[0] == ';') {
            continue;
        }
        
        sscanf(Current_Line, " %s", current_word);
        if (is_label(current_word)) {
            if (is_label(current_word) == -1) {
                printf("Error at line %d: invalid label format\n", line_counter);
                free(current_word);
                continue;
            }
            skip_to_next_word(&current_word);
        } else if (is_instruction(current_word)) {
            skip_to_next_word(&current_word);
            addres_mode = addressing_method(current_word);
            if (addres_mode == NULL || strstr(addres_mode, instruction_Table[find_in_table(current_word)].source) == NULL) {
                printf("Error at line %d: invalid addressing method or source operand\n", line_counter);
                free(current_word);
                return NULL;
            }
            skip_to_the_next_operand(&current_word);
            addres_mode = addressing_method(current_word);
            if (strstr(addres_mode, instruction_Table[find_in_table(current_word)].dest) == NULL) {
                printf("Error at line %d: invalid destination operand\n", line_counter);
                free(current_word);
                return NULL;
            }
        } else if (is_directive(current_word)) {
            if (strcmp(current_word, ".data") == 0) {
                skip_to_next_word(&current_word);
                skip_leading_sign(&current_word);
            } else {
                printf("Error at line %d: expected a number\n", line_counter);
                free(current_word);
                return NULL;
            }
        } else if (strcmp(current_word, ".string") == 0) {
            skip_to_next_word(&current_word);
            if (current_word[0] != '\"' || current_word[strlen(current_word) - 1] != '\"') {
                printf("Error at line %d: invalid string\n", line_counter);
                free(current_word);
                return NULL;
            }
        } else if (strcmp(current_word, ".entry") == 0 || strcmp(current_word, ".extern") == 0) {
            continue;
        } else {
            printf("Error at line %d: invalid instruction or directive\n", line_counter);
            free(current_word);
            return NULL;
        }
        fputs(Current_Line, temp_file);
    }
    
    fclose(temp_file);
    fclose(am_file);
    free(am_file_name);
    free(current_word);
    return NULL;
}

/* Main function */
int main() {
    char *fName = "test";
    validation(fName);
    return 0;
}
