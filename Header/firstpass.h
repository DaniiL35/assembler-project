#ifndef FIRSTPASS_H
#define FIRSTPASS_H

/* Include any necessary libraries or headers */
#include "utils.h"
#include "globals.h"

/* Declare any global variables or constants */
#define WORD_LEN 31

/* Declare struct definitions */
struct label {
    char label_Name[WORD_LEN];
    int address;
    int is_extern;  
    int is_entry;
    struct label *next;
};

struct labelTable {
    struct label *table[TABLE_SIZE];
};

/* Declare function prototypes */
void initlabelTable(struct labelTable *labelTable);

void insertLabel(struct labelTable *labelTable, struct label *label);

struct label *search_label(struct labelTable *labelTable, char *name);

int ic_calculate(char *command, char *operand1, char *operand2);

int count_numbers(char *str);

int count_letters(char *str);

int dc_calculate(char *command, char *operand1, char *operand2);

int has_label(char *label);

struct labelTable *firstpass(char *fName) ;


#endif /* FIRSTPASS_H */