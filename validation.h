#ifndef VALIDATION_H
#define VALIDATION_H


/* Constants */
#define MAX_LABEL_LENGTH 31
#define INSTRUCTION_TABLE_SIZE 16

/* Structure declarations */
typedef enum{
    false,
    true
}bool;


/* Function declarations */
int valid_register(char *str);
static int is_alphanumeric_and_upper_string(const char *str, int length) ;



#endif
