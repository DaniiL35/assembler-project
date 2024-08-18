/**
 * @file firstpass.h
 * @brief Header file for the first pass of the assembler.
 *
 * This file contains the definitions for the label table and functions used in the first
 * pass of the assembler. It includes function prototypes for initializing and managing
 * the label table, processing label definitions, and performing the first pass of
 * the assembler.
 */
#ifndef FIRSTPASS_H
#define FIRSTPASS_H

/* Include any necessary libraries or headers */
#include "utils.h"
#include "globals.h"
#include "validation.h"

/* Declare any global variables or constants */
#define WORD_LEN 31
#define MAX_MEMORY 4096


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
/**
 * @brief Skips to the .data or .string directive in a line.
 * 
 * This function skips to the .data or .string directive in a line of assembly code.
 * 
 * @param line_ptr Pointer to the line of assembly code.
 * @return Pointer to the position in the line where the .data or .string directive starts.
 */
char* skipToDataOrString(char* line_ptr);

/**
 * @brief Calculates the data counter (DC) value for a line.
 * 
 * This function calculates the data counter (DC) value for a line of assembly code.
 * 
 * @param line The line of assembly code.
 * @return The calculated DC value.
 */
int dc_calc(char *line);

/**
 * @brief Defines a label in the label table.
 * 
 * This function defines a label in the label table based on the current line of assembly code.
 * 
 * @param line_buffer The line buffer containing the line of assembly code.
 * @param currentLabel Pointer to the current label.
 * @param lTable Pointer to the label table.
 * @param ic The instruction counter.
 * @param dc The data counter.
 * @return A result indicating the success or failure of the label definition.
 */
LabelDefResult label_def(char *line_buffer, struct Label **currentLabel, struct labelTable *lTable, int ic, int dc);

/**
 * @brief Searches for a label in the label table.
 * 
 * This function searches for a label in the label table by name.
 * 
 * @param lTable Pointer to the label table.
 * @param label The name of the label to search for.
 * @return Pointer to the found label, or NULL if not found.
 */
struct Label* search_label(struct labelTable *lTable, char *label);

/**
 * @brief Inserts a label into the label table.
 * 
 * This function inserts a label into the label table.
 * 
 * @param lTable Pointer to the label table.
 * @param newLabel Pointer to the new label to insert.
 */
void insertLabel(struct labelTable *lTable, struct Label *newLabel);

/**
 * @brief Initializes the label table.
 * 
 * This function initializes the label table by setting all entries to NULL.
 * 
 * @param lTable Pointer to the label table to initialize.
 */
void initLabelTable(struct labelTable *lTable);

/**
 * @brief Performs the first pass of the assembler.
 * 
 * This function performs the first pass of the assembler on the given assembly file.
 * 
 * @param Vname The name of the output file.
 * @param fName The name of the input assembly file.
 * @return Pointer to the label table created during the first pass.
 */
struct labelTable* firstpass(char *Vname, char *fName);

/**
 * @brief Calculates the command size for two operands.
 * 
 * This function calculates the command size for two operands in an assembly instruction.
 * 
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @return The calculated command size.
 */
int command_calc(char *op1, char *op2);
#endif /* FIRSTPASS_H */