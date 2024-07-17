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


#endif
