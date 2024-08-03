#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "preprocessor.h"
#include "validation.h"




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
    int i = 1;
    int operand_len = strlen(str);
    if (str[0] == '#') { /* immediate addressing */
        skip_leading_sign(&str);
        while(*str != ',' && *str != ' ' && *str == '\0') {
            if (!isdigit(str[i])) {
                printf("Error: invalid immediate addressing\n");
                return NULL;
            }
        }
        return "0";
    }
    /* Direct Addressing */
        if(str[operand_len-1] == ','){
            operand_len--;
        }
        if (is_alphanumeric_and_upper_string(str, operand_len)) {
            return "1";
    }

    if (str[0] == '*') { /* Indirect Register Addressing */
        str++;
        if (valid_register(str)) {
            return "2";
        }
    }
    if (valid_register(str)) { /* Direct Register Addressing */
        return "3";
    }
    printf("Error: invalid addressing method\n");
    return NULL;
}

/* Function to check if a string is a valid register */
int valid_register(char *str) {
    return (str[0] == 'r' && str[1] >= '0' && str[1] <= '7' && (str[2] == ' ' || str[2] == ',' || str[2] == '\0')) ? true : false;
}

/* Function to check if a word is a directive */
int is_directive(char *word) {
    if (strcmp(word, ".data") == 0) {
        return 1;
    }
    if (strcmp(word, ".string") == 0) {
        return 2;
    }
    if (strcmp(word, ".entry") == 0) {
        return 3;
    }
    if (strcmp(word, ".extern") == 0) {
        return 4;
    }
    return false;
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
static int is_alphanumeric_and_upper_string(const char *str, int length) {
    int i;
    for (i = 0; i < length; i++) {
        if (!isalnum(str[i]) || !isupper(str[i])) {
            return false;
        }
    }
    return true;
}

/* Function to check if a word is a label */
static int is_label(char *word) {
    char label[MAX_LABEL_LENGTH];
    int label_len;
    sscanf(word, "%s", label);
    label_len = strlen(label);

    if (label[label_len - 1] == ':'){
        if(is_alphanumeric_and_upper_string(label,label_len-1)){
            return 1;
        }
        else
            return -1;
    }
    return 0;
}


/* Function to skip to the next word in a string */
void skip_to_next_word(char **str) {
    /* Skip any leading spaces */
    while (**str == ' ') {
        (*str)++;
    }

    /* Skip the current word until a space, comma, or end of string is found */
    while (**str != ' ' && **str != ',' && **str != '\0') {
        (*str)++;
    }

    /* Skip any trailing spaces after the word */
    while (**str == ' ') {
        (*str)++;
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
/* Function to skip leading spaces in a string */
void skip_leading_spaces(char **str) {
    while (**str == ' ') {
        (*str)++;
    }
}
/* Function to check if a string represents a valid integer */
int is_valid_integer(char *str) {
    if (*str == '-' || *str == '+') {
        str++;
    }
    if (!isdigit(*str)) {
        return false;
    }
    while (*str) {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}
void skip_leading_sign(char **str) {
    if (**str == '-' || **str == '+') {
        (*str)++;
    }
}

/* Function to check if a string contains valid data */
int is_valid_data(char **str) {
    while (**str != '\0') {
        skip_leading_spaces(str);
        skip_leading_sign(str);
        if (!isdigit(**str)) {
            return false;
        }
        while (isdigit(**str)) {
            (*str)++;
        }
        skip_leading_spaces(str);
        if (**str == ',') {
            (*str)++;
            skip_leading_spaces(str);
        } else if (**str != '\0') {
            return false;
        }
    }
    return true;
}




/* Function for validation */
char *validation(char *fName) {
    int line_counter = 0;
    char Current_Line[Max_LINE_LEN];
    char *line_ptr;
    char *addres_mode;
    char instruction_temp[5];
    char first_operand[Max_LINE_LEN];
    char second_operand[Max_LINE_LEN];
    char *current_word = (char *)malloc(Max_LINE_LEN * sizeof(char));
    char *am_file_name = strcatWithMalloc(fName, am_file_ext);
    FILE *am_file = openFileAndCheck(am_file_name, "r");
    FILE *temp_file = openFileAndCheck("temp.am", "w");
    printf("Reading started\n"); /* testing only */

    /* reading line by line from the am file */
    while (fgets(Current_Line, Max_LINE_LEN, am_file) != 0) {
        line_ptr = Current_Line;
        printf("Current line: %s", Current_Line); /* testing only */
        line_counter++;
        /* skip empty lines and comments */
        if (is_empty(Current_Line) || Current_Line[0] == ';') {
            continue;
        }
        /* for lines with labels */
        sscanf(Current_Line, " %s", current_word);
        if (is_label(current_word)) {
            skip_to_next_word(&line_ptr); /* skip the label */ 
            printf("line ptr after label: %s\n", line_ptr); /* testing only */    
        if (is_label(current_word) == -1) {
                fprintf(stdout, "Error at line %d: invalid label format\n", line_counter);
                free(current_word);
                continue;
            }
        }
        else
            printf("No label\n"); /* testing only */

        /* for lines without labels or after the label word */
        /* check if the word is an instruction */
        sscanf(line_ptr, " %s", current_word);
        if (is_instruction(current_word)) {
            printf("instruction found: %s\n", current_word); /* testing only */
            printf("Initial Current_Line: %s\n", Current_Line); /* Debugging */
            strcpy(instruction_temp, current_word);
            printf(" Current_Line: after strcpy: %s\n", Current_Line); /* Debugging */
            printf ("instruction_temp: %s\n", instruction_temp); /* testing only */
            printf("number of operands: %d\n", instruction_Table[find_in_table(current_word)].
            num_of_operands); /* testing only */
            switch (instruction_Table[find_in_table(current_word)].num_of_operands) {
                case 0: /* Code for instructions with 0 operands */
                    printf("Processing instruction with 0 operands: %s\n", Current_Line); /* Debugging */
                    fputs(Current_Line, temp_file);
                    break;
                case 1: /* Code for instructions with 1 operand */
                    skip_to_next_word(&line_ptr);
                    sscanf(line_ptr, " %s", current_word);
                    printf("current word: %s\n", current_word); /* testing only */
                    addres_mode = addressing_method(current_word);
                    printf("addressing mode: %s\n", addres_mode); /* testing only */
                    
                    /* Check if the source operand is empty */
                    if (strcmp(instruction_Table[find_in_table(instruction_temp)].source, "") == 0) {
                        /* Check the destination operand */
                        if (strstr(instruction_Table[find_in_table(instruction_temp)].dest, addres_mode) != NULL) {
                            fputs(Current_Line, temp_file);
                        } else {
                            fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                            free(current_word);
                            return NULL;
                        }
                    } else {
                        /* Check the source operand */
                        if (strstr(instruction_Table[find_in_table(instruction_temp)].source, addres_mode) != NULL) {
                            fputs(Current_Line, temp_file);
                        } else {
                            fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                            free(current_word);
                            return NULL;
                        }
                    }
                    break;
                case 2: /* Code for instructions with 2 operands */
                    skip_to_next_word(&line_ptr);
                    printf("line_ptr: %s\n", line_ptr); /* testing only */
                    sscanf(line_ptr, " %s", first_operand);
                    printf("first operand: %s\n", first_operand); /* testing only */
                    addres_mode = addressing_method(first_operand);
                    printf("addressing mode of source: %s\n", addres_mode); /* testing only */
                    printf("source: %s\n", instruction_Table[find_in_table(instruction_temp)].source); /* testing only */
                    if (strstr(instruction_Table[find_in_table(instruction_temp)].source, addres_mode) == NULL) { /* check the source operand */
                        fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                        free(current_word);
                        return NULL;
                    }
                    skip_to_the_next_operand(&line_ptr);
                    sscanf(line_ptr, " %s", current_word);
                    printf("2nd operand: %s\n", current_word); /* testing only */                    
                    addres_mode = addressing_method(current_word);
                    printf("addressing mode of dest: %s\n", addres_mode); /* testing only */
                    if (strstr(instruction_Table[find_in_table(instruction_temp)].dest, addres_mode) == NULL) { /* check the destination operand */
                        fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                        free(current_word);
                        return NULL;
                    }
                    printf("checked both operands\n"); /* testing only */
                    fputs(Current_Line, temp_file);
                    break;
                default:
                    printf("Error at line %d: invalid number of operands\n", line_counter);
                    free(current_word);
                    return NULL;
            }
        } else if (is_directive(current_word)) {
            switch (is_directive(current_word)) {
                case 1: /* Code for .data directive */
                    skip_to_next_word(&current_word);
                    if (!is_valid_data(&current_word)) {
                        fprintf(stdout, "Error at line %d: invalid data\n", line_counter);
                        free(current_word);
                        return NULL;
                    }
                    fputs(Current_Line, temp_file);
                    break;
                case 2: /* Code for .string directive */
                    skip_to_next_word(&current_word);
                    
                    break;
                case 3: /* Code for .entry directive */
                    skip_to_next_word(&current_word);
                    if (!is_label(current_word)) {
                        fprintf(stdout, "Error at line %d: invalid label format\n", line_counter);
                        free(current_word);
                        return NULL;
                    }
                    fputs(Current_Line, temp_file);
                    break;
                case 4: /* Code for .extern directive */
                    skip_to_next_word(&current_word);
                    if (!is_label(current_word)) {
                        fprintf(stdout, "Error at line %d: invalid label format\n", line_counter);
                        free(current_word);
                        return NULL;
                    }
                    fputs(Current_Line, temp_file);
                    break;
                default:
                    fprintf(stdout, "Error at line %d: invalid directive\n", line_counter);
                    free(current_word);
                    return NULL;
            }
        } else {
            fprintf(stdout, "Error at line %d: invalid instruction or directive\n", line_counter);
            free(current_word);
            return NULL;
        }
    printf("finished line %d\n", line_counter); /* testing only */
    printf("finished validate line %d\n", line_counter); /* testing only */    


    
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
