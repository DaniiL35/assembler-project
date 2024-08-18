/**
 * @file secondpass.h
 * @brief Header file for the second pass of the assembler.
 * 
 * This file declares functions for the second pass of the assembler, which include converting
 * commands to binary format, handling labels, and managing output files.
 */
#ifndef SECONDPASS_H
#define SECONDPASS_H

/* Include any necessary libraries or headers */
#include "utils.h"
#include "firstpass.h"
#include "globals.h"


/* Declare any global variables or constants */
#define COM_TABLE_SIZE 18

/* Structure to represent a command */
struct command{
    char *name;
    int opcode;
    int num_of_operands;
};



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

typedef struct {
    unsigned int data : 15;   /**< Data field */
} data_bitField;


/**
 * @brief Converts a 15-bit binary string to its octal representation.
 * 
 * @param binary_str The binary string to convert.
 * @return A pointer to the octal representation of the binary string.
 */
char* binary_to_octal(const char* binary_str);

/**
 * @brief Gets the ASCII value of a character.
 * 
 * @param c The character to convert.
 * @return The ASCII value of the character.
 */
int getAsciiValue(char c);

/**
 * @brief Prints labels marked as entry to the entry file.
 * 
 * @param lTable Pointer to the label table.
 * @param ent_file Pointer to the entry file.
 */
void printLabelToEntry(struct labelTable *lTable, FILE *ent_file);

/**
 * @brief Prints labels marked as extern to the extern file.
 * 
 * @param lTable Pointer to the label table.
 * @param ext_file Pointer to the extern file.
 * @param operand1 The first operand.
 * @param operand2 The second operand.
 * @param ic Pointer to the instruction counter.
 */
void printLabelToExtern(struct labelTable *lTable, FILE *ext_file, char *operand1, char *operand2, int *ic);

/**
 * @brief Trims whitespace from the beginning and end of a string.
 * 
 * @param str The string to trim.
 * @return A pointer to the trimmed string.
 */
char* trim_whitespace(char* str);

/**
 * @brief Decodes a string command and writes its binary representation to the object file.
 * 
 * @param operand The string to decode.
 * @param IC The current instruction counter.
 * @param ob_file Pointer to the object file.
 * @return The number of bytes processed.
 */
int decode_string(char *operand, int IC, FILE *ob_file);

/**
 * @brief Decodes data commands and writes their binary representation to the object file.
 * 
 * @param operand The data command to decode.
 * @param IC The current instruction counter.
 * @param ob_file Pointer to the object file.
 * @return The number of bytes processed.
 */
int decode_data(char *operand, int IC, FILE *ob_file);

/**
 * @brief Extracts a number from a string.
 * 
 * @param str The string containing the number.
 * @return The extracted number.
 */
int extract_number(char* str);

/**
 * @brief Gets the opcode of a command from the instruction table.
 * 
 * @param command The command to get the opcode for.
 * @return The opcode of the command.
 */
int getOpcode(char *command);

/**
 * @brief Gets the addressing mode of an operand.
 * 
 * @param operand The operand to check.
 * @return The addressing mode.
 */
int get_adressing(char *operand);

/**
 * @brief Gets the ARE (Addressing Register/Immediate) value from an operand.
 * 
 * @param operand The operand to check.
 * @return The ARE value.
 */
int get_ARE(char *operand);

/**
 * @brief Combines bit fields into a single unsigned integer.
 * 
 * @param bf The bit fields to combine.
 * @return The combined bit field as an unsigned integer.
 */
unsigned int combineBitField(BitField bf);

/**
 * @brief Combines register bit fields into a single unsigned integer.
 * 
 * @param op_bf The register bit fields to combine.
 * @return The combined register bit field as an unsigned integer.
 */
unsigned int combineRegBitField(reg_bitField op_bf);

/**
 * @brief Combines label bit fields into a single unsigned integer.
 * 
 * @param op_bf The label bit fields to combine.
 * @return The combined label bit field as an unsigned integer.
 */
unsigned int combineLabelBitField(label_bitField op_bf);

/**
 * @brief Converts an unsigned integer to a binary string.
 * 
 * @param num The number to convert.
 * @param str The string to store the binary representation.
 * @param length The length of the binary string.
 */
void toBinaryString(unsigned int num, char *str, int length);

/**
 * @brief Gets the number of operands for a given command from the instruction table.
 * 
 * @param cmd The command to check.
 * @return The number of operands.
 */
int number_of_operands(char *cmd);

/**
 * @brief Converts a command line to binary and writes it to the object file.
 * 
 * @param line The command line to process.
 * @param labelTable Pointer to the label table.
 * @param ob_file Pointer to the object file.
 * @param ext_file Pointer to the extern file.
 * @param IC The current instruction counter.
 * @return The number of bytes processed.
 */
int commandToBinary(char *line, struct labelTable *labelTable, FILE *ob_file, FILE *ext_file, int IC);

/**
 * @brief Converts an operand to binary and writes it to the object file.
 * 
 * @param op The operand to convert.
 * @param labelTable Pointer to the label table.
 * @param ob_file Pointer to the object file.
 * @param IC The current instruction counter.
 */
void opToBinary(char *op, struct labelTable *labelTable, FILE *ob_file, int IC);

/**
 * @brief Converts two operands to binary and writes them to the object file.
 * 
 * @param op1 The first operand.
 * @param op2 The second operand.
 * @param labelTable Pointer to the label table.
 * @param ob_file Pointer to the object file.
 * @param IC The current instruction counter.
 * @return The number of bytes processed.
 */
int opToBinaryDouble(char *op1, char *op2, struct labelTable *labelTable, FILE *ob_file, int IC);

/**
 * @brief Performs the second pass of the assembler.
 * 
 * @param validatedFileName The name of the validated file.
 * @param labelTable Pointer to the label table.
 * @param originalFileName The name of the original file.
 * @return A pointer to the result of the second pass.
 */
int* secondpass(char *validatedFileName, struct labelTable *labelTable, char *originalFileName);

#endif


