/**
 * @file preprocessor.h
 * @brief Header file for the preprocessor.
 *
 * This file contains the definitions for the macro structures, macro table, and functions
 * used for macro processing in the preprocessor. It includes function prototypes for
 * initializing and managing the macro table, processing macro definitions and calls,
 * and preprocessing assembly files.
 */
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "utils.h"

/* Constants */

#define Max_macro_len_name  256
#define Macro_start         "macr"
#define Macro_end           "endmacr"
#define invalid_macro_table_size 30

/* Structure declarations */
struct Macro;
struct MacroTable;



/* Function declarations */
/**
 * @brief Initialize the macro table.
 * 
 * This function sets all entries in the macro table to NULL.
 * 
 * @param mTable Pointer to the macro table to initialize.
 */
void initMacroTable(struct MacroTable *mTable);

/**
 * @brief Free the memory used by the macro table.
 * 
 * This function deallocates memory for all macros and their associated lines in the macro table.
 * 
 * @param mTable Pointer to the macro table to free.
 */
void freeMacroTable(struct MacroTable *mTable);

/**
 * @brief Insert a macro into the macro table.
 * 
 * This function adds a macro to the macro table at the appropriate index.
 * 
 * @param mTable Pointer to the macro table.
 * @param macro Pointer to the macro to insert.
 */
void insertMacro(struct MacroTable *mTable, struct Macro *macro);

/**
 * @brief Search for a macro in the macro table.
 * 
 * This function searches the macro table for a macro with a specific name.
 * 
 * @param mTable Pointer to the macro table.
 * @param name Name of the macro to search for.
 * @return Pointer to the found macro, or NULL if the macro is not found.
 */
struct Macro *search_macro(struct MacroTable *mTable, char *name);

/**
 * @brief Process a line from the source file and update the current macro.
 * 
 * This function processes a line of text to determine if it is the start of a macro definition,
 * the end of a macro definition, a macro call, or a normal line. It updates the current macro
 * accordingly.
 * 
 * @param line_buffer Line from the source file.
 * @param currentMacro Pointer to the current macro being processed.
 * @param mTable Pointer to the macro table.
 * @return Status code indicating the type of line (macro definition, macro call, etc.).
 */
int macro_line(char *line_buffer, struct Macro **currentMacro, struct MacroTable *mTable);

/**
 * @brief Preprocess the given assembly file.
 * 
 * This function reads an assembly source file, processes macro definitions and calls, and
 * writes the preprocessed output to a new file. The new file's name is based on the input file's
 * base name with an appropriate extension.
 * 
 * @param fName Base name of the assembly file (without extension).
 * @return Name of the preprocessed file, or "error" if an error occurred.
 */
char *preprocessor(char *fName);
#endif
