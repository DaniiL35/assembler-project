#ifndef SECONDPASS_H
#define SECONDPASS_H

/* Include any necessary libraries or headers */
#include "utils.h"
#include "firstpass.h"
#include "globals.h"


/* Declare any global variables or constants */

/**
 * @struct BitField
 * @brief A structure to represent a bit field.
 */
typedef struct bitField {
    unsigned int opcode : 4;  /**< Opcode field */
    unsigned int src : 4;     /**< Source field */
    unsigned int dest : 4;    /**< Destination field */
    unsigned int are : 3;     /**< ARE field */
} BitField;

/**
 * @struct reg_bitField
 * @brief A structure to represent a register bit field.
 */
typedef struct reg_bitField {
    unsigned int free : 6;    /**< Free bits */
    unsigned int src : 3;     /**< Source field */
    unsigned int dest : 3;    /**< Destination field */
    unsigned int are : 3;     /**< ARE field */
} reg_bitField;

/**
 * @struct label_bitField
 * @brief A structure to represent a label bit field.
 */
typedef struct label_bitField {   
    unsigned int dest : 12;   /**< Destination field */
    unsigned int are : 3;     /**< ARE field */
} label_bitField;


/* Function declaration for secondpass */
/**
 * @brief Converts a binary string to an octal string.
 * @param binary_str The binary string to convert.
 * @return The octal string.
 */
char* binary_to_octal(const char* binary_str);

/**
 * @brief Prints a label to the entry file.
 * @param lTable The label table.
 * @param ent_file The entry file.
 */
void printLabelToEntry(struct labelTable *lTable, FILE *ent_file);

/**
 * @brief Prints a label to the extern file.
 * @param lTable The label table.
 * @param ext_file The extern file.
 * @param operand1 The first operand.
 * @param operand2 The second operand.
 * @param ic The instruction counter.
 */
void printLabelToExtern(struct labelTable *lTable, FILE *ext_file, char *operand1, char *operand2, int *ic);

/**
 * @brief Extracts a number from a string.
 * @param str The string to extract the number from.
 * @return The extracted number.
 */
int extract_number(char* str);

/**
 * @brief Gets the opcode for a command.
 * @param command The command to get the opcode for.
 * @return The opcode.
 */
int getOpcode(char *command);

/**
 * @brief Gets the addressing mode for an operand.
 * @param operand The operand to get the addressing mode for.
 * @return The addressing mode.
 */
int get_adressing(char *operand);

/**
 * @brief Gets the ARE field for an operand.
 * @param operand The operand to get the ARE field for.
 * @return The ARE field.
 */
int get_ARE(char *operand);

/**
 * @brief Combines a BitField structure into an unsigned int.
 * @param bf The BitField structure to combine.
 * @return The combined unsigned int.
 */
unsigned int combineBitField(BitField bf);

/**
 * @brief Combines a reg_bitField structure into an unsigned int.
 * @param op_bf The reg_bitField structure to combine.
 * @return The combined unsigned int.
 */
unsigned int combineRegBitField(reg_bitField op_bf);

/**
 * @brief Combines a label_bitField structure into an unsigned int.
 * @param op_bf The label_bitField structure to combine.
 * @return The combined unsigned int.
 */
unsigned int combineLabelBitField(label_bitField op_bf);

/**
 * @brief Converts a number to a binary string.
 * @param num The number to convert.
 * @param str The string to store the binary representation.
 * @param length The length of the binary string.
 */
void toBinaryString(unsigned int num, char *str, int length);

/**
 * @brief Gets the number of operands for a command.
 * @param cmd The command to get the number of operands for.
 * @return The number of operands.
 */
int number_of_operands(char *cmd);

/**
 * @brief Converts a command to binary.
 * @param line The command line to convert.
 * @param labelTable The label table.
 * @param ob_file The object file.
 * @param IC The instruction counter.
 * @return The status of the conversion.
 */
int commandToBinary(char *line, struct labelTable *labelTable, FILE *ob_file, int IC);

/**
 * @brief Converts an operand to binary.
 * @param op The operand to convert.
 * @param labelTable The label table.
 * @param ob_file The object file.
 * @param IC The instruction counter.
 */
void opToBinary(char *op, struct labelTable *labelTable, FILE *ob_file, int IC);

/**
 * @brief Converts two operands to binary.
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param labelTable The label table.
 * @param ob_file The object file.
 * @param IC The instruction counter.
 * @return The status of the conversion.
 */
int opToBinaryDouble(char *op1, char *op2, struct labelTable *labelTable, FILE *ob_file, int IC);

/**
 * @brief Performs the second pass of the assembler.
 * @param validatedFileName The validated file name.
 * @param labelTable The label table.
 * @param originalFileName The original file name.
 * @return The status of the second pass.
 */
int* secondpass(char *validatedFileName, struct labelTable *labelTable, char *originalFileName);

#endif


