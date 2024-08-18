#ifndef VALIDATION_H
#define VALIDATION_H


#include "utils.h"
#include "reordering.h"


/* Constants */
#define MAX_LABEL_LENGTH 31
#define INSTRUCTION_TABLE_SIZE 16
#define REGISTER_TABLE_SIZE 8
#define DIRECTIVE_TABLE_SIZE 5
#define INSTRUCTION_LENGTH 5
#define operand_length 30




/* Structure to represent an instruction */
struct instruction {
    char *name;
    int opcode;
    const char *dest;
    const char *source;
    int num_of_operands;
};

extern struct instruction instruction_Table[INSTRUCTION_TABLE_SIZE]; 


/* Function declarations */
/**
 * @brief Checks if a string is a valid register.
 * @param str The string to check.
 * @return 1 if the string is a valid register, 0 otherwise.
 */
int valid_register(char *str);

/**
 * @brief Checks if a string is alphanumeric.
 * @param str The string to check.
 * @param length The length of the string.
 * @return 1 if the string is alphanumeric, 0 otherwise.
 */
int is_alphanumeric_string(const char *str, int length);

/**
 * @brief Skips leading sign characters in a string.
 * @param str The string to modify.
 */
void skip_leading_sign(char **str);

/**
 * @brief Skips to the next word in a string.
 * @param str The string to modify.
 */
void skip_to_next_word(char **str);

/**
 * @brief Skips to the next operand in a string.
 * @param str The string to modify.
 */
void skip_to_the_next_operand(char **str);

/**
 * @brief Skips leading spaces in a string.
 * @param str The string to modify.
 */
void skip_leading_spaces(char **str);

/**
 * @brief Checks if a string is a valid integer.
 * @param str The string to check.
 * @return 1 if the string is a valid integer, 0 otherwise.
 */
int is_valid_integer(char *str);

/**
 * @brief Checks if a string is valid data.
 * @param str The string to check.
 * @return 1 if the string is valid data, 0 otherwise.
 */
int is_valid_data(char **str);

/**
 * @brief Checks if a word is a directive.
 * @param word The word to check.
 * @return An integer representing the directive type:
 *         1 for .data,
 *         2 for .string,
 *         3 for .entry,
 *         4 for .extern,
 *         0 if the word is not a directive.
 */
int is_directive(char *word);

/**
 * @brief Checks if a line is empty.
 * @param line The line to check.
 * @return 1 if the line is empty, 0 otherwise.
 */
int is_empty(const char *line);

/**
 * @brief Checks if a word is an instruction.
 * @param word The word to check.
 * @return 1 if the word is an instruction, 0 otherwise.
 */
int is_instruction(char *word);

/**
 * @brief Checks if a word is a label.
 * @param word The word to check.
 * @return 1 if the word is a label, 0 otherwise.
 */
int is_label(char *word);

/**
 * @brief Determines the addressing method of an operand.
 * @param str The operand string.
 * @return A string representing the addressing method.
 */
char *addressing_method(char *str);

/**
 * @brief Finds a word in the instruction table.
 * @param word The word to find.
 * @return The index of the word in the instruction table, or -1 if not found.
 */
int find_in_instruction_Table_table(char *word);

/**
 * @brief Validates the given file.
 * @param fName The name of the file to validate.
 * @return A pointer to the validated file name.
 */
char *validation(char *fName);

#endif