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
int find_in_instruction_Table_table(char *word) {
    int i;
    for (i = 0; i < INSTRUCTION_TABLE_SIZE; i++) {
        if (strcmp(word, instruction_Table[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

/* Function to determine the addressing method of an operand */
/* Function to determine the addressing method of an operand */
char *addressing_method(char *str) {
    int i = 1;
    int operand_len = strlen(str);
    if (str[0] == '#' && operand_len >= 2) { /* immediate addressing */
        skip_leading_sign(&str);
        while(*str != ',' && *str != ' ' && *str == '\0') {
            if (!isdigit(str[i])) {
                printf("Error: invalid immediate addressing\n");
                return NULL;
            }
        }
        return "0";
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
    /* Direct Addressing */
    if(str[operand_len-1] == ','){
        operand_len--;
    }
    if (is_alphanumeric_string(str, operand_len-1)) {
        return "1";
    }
    /* invalid addressing method */
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
    return find_in_instruction_Table_table(word) != -1;
}

/* Function to check if a string is alphanumeric */
 int is_alphanumeric_string(const char *str, int length) {
    int i;
    for (i = 0; i < length; i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }
    return true;
}

/* Function to check if a word is a label */
int is_label(char *word) {
    int len = strlen(word);
    int i;

    if (word == NULL) {
        return false;
    }

    if (len < 2) {
        return false;
    }

    if (word[len - 1] != ':') {
        return false;
    }

    for (i = 0; i < len - 1; i++) {
        if (!isalnum(word[i])) {
            return false;
        }
    }

    return true;
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

void skip_leading_sign(char **str) {
    if (**str == '-' || **str == '+') {
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
/* Function to validate that a string has exactly two quotes and contains only valid characters inside the quotes */
int validate_string(char *line) {
    int len = strlen(line);
    int i;
    
    /* Check if the string starts and ends with a quote */
    if (line[0] != '"' || line[len - 2] != '"') {
        return false;
    }

    /* Check that characters between quotes are alphabetic */
    for (i = 1; i < len - 3; i++) {
        if (!isalpha(line[i])) {
            return false;
        }
    }

    return true;
}
/* Function to remove the trailing comma from a string */
void remove_comma(char *str) {
    size_t len = strlen(str);
    if (str[len - 1] == ',') {
        str[len - 1] = '\0';
    }
}

/* Function to remove trailing newline characters from a string */
void remove_trailing_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}


/* Function for validation */
char *validation(char *fName) {
    int line_counter = 0;
    int error_flag = 0;
    char Current_Line[MAX_LINE_LEN];
    char *line_ptr;
    char *addres_mode;
    char *quote_ptr;
    char label[MAX_LINE_LEN];
    char instruction_temp[5];
    char first_operand[MAX_LINE_LEN];
    char second_operand[MAX_LINE_LEN];
    char directive_temp[10];
    char formated_line[9000];
    char *original_current_word;
    char *validateFileName = strcatWithMalloc(fName, "_validate.am");
    char *current_word = (char *)malloc(MAX_LINE_LEN * sizeof(char));
    char *am_file_name = strcatWithMalloc(fName, AM_FILE_EXT);
    FILE *am_file = openFileAndCheck(am_file_name, "r");
    FILE *temp_file = openFileAndCheck("temp.am", "w");
    char *tempFileName = malloc(strlen(am_file_name) + 1);
    printf("Reading started\n"); /* testing only */

    /* reading line by line from the am file */
    while (fgets(Current_Line, MAX_LINE_LEN, am_file) != 0) {
        line_ptr = Current_Line; /* set the pointer to the beginning of the line */
        printf("Current line: %s\n", Current_Line); /* testing only */
        line_counter++; /* increment the line counter */

        /* skip empty lines and comments */
        if (is_empty(Current_Line) || Current_Line[0] == ';') {
            continue;
        }

        /* Copy of current_word to preserve original pointer */
         original_current_word = current_word;
        sscanf(Current_Line, " %s", current_word);

        /* for lines with labels */
        if (is_label(current_word)) {
            strcpy(label, current_word);
            skip_to_next_word(&line_ptr); /* skip the label */
            printf("line ptr after label: %s\n", line_ptr); /* testing only */
        } else {
            /* if the line doesn't have a label */
            strcpy(label, "0");
        }

        /* for lines without labels or after the label word */
        /* check if the word is an instruction */
        sscanf(line_ptr, " %s", current_word);
        if (is_instruction(current_word)) {
            printf("instruction found: %s\n", current_word); /* testing only */
            printf("Initial Current_Line: %s\n", Current_Line); /* Debugging */
            strcpy(instruction_temp, current_word);
            printf(" Current_Line: after strcpy: %s\n", Current_Line); /* Debugging */
            printf("instruction_temp: %s\n", instruction_temp); /* testing only */
            printf("number of operands: %d\n", instruction_Table[find_in_instruction_Table_table(current_word)].
                   num_of_operands); /* testing only */
            switch (instruction_Table[find_in_instruction_Table_table(current_word)].num_of_operands) {
                case 0: /* Code for instructions with 0 operands */
                    printf("Processing instruction with 0 operands: %s\n", Current_Line); /* Debugging */
                    sprintf(formated_line, "%s %s\n", label, instruction_temp);
                    printf("Formatted line: %s\n", formated_line);
                    fputs(formated_line, temp_file);
                    break;
                case 1: /* Code for instructions with 1 operand */
                    /* prep */
                    skip_to_next_word(&line_ptr);
                    sscanf(line_ptr, " %s", current_word);
                    printf("current word: %s\n", current_word); /* testing only */
                    addres_mode = addressing_method(current_word);
                    printf("addressing mode: %s\n", addres_mode); /* testing only */
                    strcpy(first_operand, current_word);

                    /* Check if the addressing method is invalid */
                    if (addres_mode == NULL) {
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                        continue;
                    }

                    /* Check if the source operand is empty */
                    if (strcmp(instruction_Table[find_in_instruction_Table_table(instruction_temp)].source, "") == 0) {
                        /* Check the destination operand */
                        if (strstr(instruction_Table[find_in_instruction_Table_table(instruction_temp)].dest, addres_mode) != NULL) {
                            sprintf(formated_line, "%s %s %s\n", label, instruction_temp, first_operand);
                            printf("formated line: %s\n", formated_line); /* testing only */
                            fputs(formated_line, temp_file);
                        } else {
                            error_flag = 1;
                            fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                            continue;
                        }
                    } else {
                        /* Check the source operand */
                        if (strstr(instruction_Table[find_in_instruction_Table_table(instruction_temp)].source, addres_mode) != NULL) {
                            sprintf(formated_line, "%s %s %s\n", label, instruction_temp, first_operand);
                            fputs(Current_Line, temp_file);
                        } else {
                            error_flag = 1;
                            fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                            continue;
                        }
                    }
                    break;
                case 2: /* Code for instructions with 2 operands */
                    /* prep */
                    skip_to_next_word(&line_ptr);
                    printf("line_ptr: %s\n", line_ptr); /* testing only */
                    sscanf(line_ptr, " %s", first_operand);
                    printf("first operand: %s\n", first_operand); /* testing only */
                    addres_mode = addressing_method(first_operand);
                    printf("addressing mode of source: %s\n", addres_mode); /* testing only */
                    printf("source: %s\n", instruction_Table[find_in_instruction_Table_table(instruction_temp)].source); /* testing only */

                    /* Check if the addressing method is invalid */
                    if (addres_mode == NULL) {
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                        continue;
                    }

                    if (strstr(instruction_Table[find_in_instruction_Table_table(instruction_temp)].source, addres_mode) == NULL) { /* check the source operand */
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                        continue;
                    }
                    if (first_operand[strlen(first_operand) - 1] != ',') {
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: missing comma between operands \n", line_counter);
                        continue;
                    }
                    skip_to_the_next_operand(&line_ptr);
                    sscanf(line_ptr, " %s", second_operand);
                    printf("2nd operand: %s\n", second_operand); /* testing only */
                    addres_mode = addressing_method(second_operand);
                    printf("addressing mode of dest: %s\n", addres_mode); /* testing only */
                    if (strstr(instruction_Table[find_in_instruction_Table_table(instruction_temp)].dest, addres_mode) == NULL) { /* check the destination operand */
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: invalid addressing method\n", line_counter);
                        continue;
                    }
                    printf("checked both operands\n"); /* testing only */
                    remove_comma(first_operand);
                    sprintf(formated_line, "%s %s %s %s\n", label, instruction_temp, first_operand, second_operand);
                    fputs(formated_line, temp_file);
                    break;
                default:
                    error_flag = 1;
                    printf("Error at line %d: invalid number of operands\n", line_counter);
                    continue;
            }
        } else if (is_directive(current_word)) {
            switch (is_directive(current_word)) {
                case 1: /* Code for .data directive */
                    strcpy(directive_temp, current_word);
                    skip_to_next_word(&original_current_word); /* Use original pointer */
                    if (!is_valid_data(&original_current_word)) {
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: invalid data\n", line_counter);
                        continue;
                    }
                    sprintf(formated_line, "%s %s %s\n", label, directive_temp, original_current_word);
                    fputs(formated_line, temp_file);
                    break;
                case 2: /* Code for .string directive */
                    strcpy(directive_temp, current_word);
                    quote_ptr = strchr(line_ptr, '\"');
                    strcpy(first_operand, quote_ptr);
                    printf("quote_ptr: %s\n", quote_ptr); /* testing only */
                    if (!validate_string(quote_ptr)) {
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: invalid string\n", line_counter);
                        continue;
                    }
                    remove_trailing_newline(first_operand);
                    sprintf(formated_line, "%s %s %s\n", label, directive_temp, first_operand);
                    fputs(formated_line, temp_file);
                    break;
                case 3: /* Code for .entry directive */
                  skip_to_next_word(&line_ptr);
                    addres_mode = addressing_method(line_ptr);
                    if (strcmp(addres_mode, "1") != 0) {
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: invalid entry code\n", line_counter);
                        continue;
                    }
                    sprintf(formated_line, "%s %s %s", label, current_word, line_ptr);
                    fputs(formated_line, temp_file);
                    break;

                case 4: /* Code for .extern directive */
                    skip_to_next_word(&line_ptr);
                    addres_mode = addressing_method(line_ptr);
                    if (strcmp(addres_mode, "1") != 0) {
                        error_flag = 1;
                        fprintf(stdout, "Error at line %d: invalid extern code\n", line_counter);
                        continue;
                    }
                    sprintf(formated_line, "%s %s %s\n", label, current_word, line_ptr);
                    fputs(formated_line, temp_file);
                    break;
                default:
                    error_flag = 1;
                    fprintf(stdout, "Error at line %d: invalid directive\n", line_counter);
                    continue;
            }
        } else {
            error_flag = 1;
            fprintf(stdout, "Error at line %d: invalid instruction or directive\n", line_counter);
            continue;
        }
        printf("finished validate line %d\n", line_counter); /* testing only */
    }

    fclose(temp_file);
    fclose(am_file);

    if (error_flag) {
        remove("temp.am");
        free(am_file_name);
        free(tempFileName);
        free(validateFileName);
        free(current_word); /* Free the dynamically allocated memory for current_word */
        printf("Error found in validation \n");
        return "error";
    }

    /* Reordering lines */
    reorder_lines("temp.am", validateFileName);

    /* Free allocated memory */
    free(current_word); /* Free the dynamically allocated memory for current_word */
    free(am_file_name);
    free(tempFileName);


    printf("Validation completed successfully\n");
    return validateFileName;
}