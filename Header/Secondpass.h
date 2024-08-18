#ifndef SECONDPASS_H
#define SECONDPASS_H

/* Include any necessary libraries or headers */
#include "utils.h"
#include "firstpass.h"
#include "globals.h"


/* Declare any global variables or constants */

/* Bit field structure */
typedef struct bitField {
    unsigned int opcode : 4;        
    unsigned int src : 4;           
    unsigned int dest : 4;          
    unsigned int are : 3;           
} BitField;

typedef struct reg_bitField {
    unsigned int free : 6;        
    unsigned int src : 3;           
    unsigned int dest : 3;          
    unsigned int are : 3;           
} reg_bitField;

typedef struct label_bitField {   
    unsigned int dest : 12;          
    unsigned int are : 3;           
} label_bitField;


/* Function declaration for secondpass */
char* binary_to_octal(const char* binary_str);
void printLabelToEntry(struct labelTable *lTable, FILE *ent_file);
void printLabelToExtern(struct labelTable *lTable, FILE *ext_file, char *operand1, char *operand2, int *ic);
int extract_number(char* str);
int getOpcode(char *command);
int get_adressing(char *operand);
int get_ARE(char *operand);
unsigned int combineBitField(BitField bf);
unsigned int combineRegBitField(reg_bitField op_bf);
unsigned int combineLabelBitField(label_bitField op_bf);
void toBinaryString(unsigned int num, char *str, int length);
int number_of_operands(char *cmd);
int commandToBinary(char *line, struct labelTable *labelTable, FILE *ob_file, int IC);
void opToBinary(char *op, struct labelTable *labelTable, FILE *ob_file, int IC);
int opToBinaryDouble(char *op1, char *op2, struct labelTable *labelTable, FILE *ob_file, int IC);
#endif
