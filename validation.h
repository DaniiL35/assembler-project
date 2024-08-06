#ifndef VALIDATION_H
#define VALIDATION_H

/* Constants */
#define MAX_LABEL_LENGTH 31
#define INSTRUCTION_TABLE_SIZE 16
#define REGISTER_TABLE_SIZE 8
#define DIRECTIVE_TABLE_SIZE 5
#define INSTRUCTION_LENGTH 5
#define operand_length 30

/* Structure declarations */
typedef enum {
    false,
    true
} bool;

/* Structure to represent an instruction */
struct instruction {
    char *name;
    int opcode;
    const char *dest;
    const char *source;
    int num_of_operands;
};

/* Function declarations */
int valid_register(char *str);
static int is_alphanumeric_string(const char *str, int length);
void skip_leading_sign(char **str);
void skip_to_next_word(char **str);
void skip_to_the_next_operand(char **str);
void skip_leading_spaces(char **str);
int is_valid_integer(char *str);
int is_valid_data(char **str);
int is_directive(char *word);
int is_empty(const char *line);
int is_instruction(char *word);
static int is_label(char *word);
char *addressing_method(char *str);
int find_in_table(char *word);
char *validation(char *fName);

#endif