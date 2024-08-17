#ifndef FIRSTPASS_H
#define FIRSTPASS_H

/* Include any necessary libraries or headers */
#include "utils.h"
#include "globals.h"
#include "validation.h"

/* Declare any global variables or constants */
#define WORD_LEN 31

/* Label structure */
struct Label {
    char name[WORD_LEN];
    int address;
    int is_extern;  
    int is_entry;
    struct Label *next;
};

/* Label table structure */
struct labelTable {
    struct Label *table[TABLE_SIZE];
    int count;
};

typedef enum {
    com,
    dat
} type_of_line;

typedef struct {
    type_of_line type;
    int number; 
} LabelDefResult;

/* Declare function prototypes */
char* skipToDataOrString(char* line_ptr);
int dc_calc(char *line);
LabelDefResult label_def(char *line_buffer, struct Label **currentLabel, struct labelTable *lTable, int ic, int dc);
struct Label* search_label(struct labelTable *lTable, char *label);
void insertLabel(struct labelTable *lTable, struct Label *newLabel);
void initLabelTable(struct labelTable *lTable);
struct labelTable* firstpass(char *Vname, char *fName);
int command_calc(char *op1, char *op2);

#endif /* FIRSTPASS_H */