#ifndef FIRSTPASS_H
#define FIRSTPASS_H

/* Include any necessary libraries or headers */
#include "utils.h"

/* Declare any global variables or constants */
#define WORD_LEN 31

/* Declare struct definitions */
struct Label {
    char name[WORD_LEN];
    int address;
    int is_extern;  
    int entry;
    struct Label *next;
};

struct labelTable {
    struct Label *table[100];
};

/* Declare function prototypes */
void insertLabel(struct labelTable *labelTable, struct Label *label);
struct Label *search_label(struct labelTable *labelTable, char *name);
int count_numbers(char *str);
int count_letters(char *str);

#endif /* FIRSTPASS_H */