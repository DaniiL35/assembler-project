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
 * @brief Initializes the macro table.
 * 
 * This function initializes the macro table by setting all entries to NULL.
 * 
 * @param mTable Pointer to the macro table to initialize.
 */
void initMacroTable(struct MacroTable *mTable);

/**
 * @brief Inserts a macro into the macro table.
 * 
 * This function inserts a macro into the macro table using a hash function to determine the index.
 * 
 * @param mTable Pointer to the macro table.
 * @param macro Pointer to the macro to insert.
 */
void insertMacro(struct MacroTable *mTable, struct Macro *macro);

/**
 * @brief Searches for a macro in the macro table.
 * 
 * This function searches for a macro in the macro table by name.
 * 
 * @param mTable Pointer to the macro table.
 * @param name The name of the macro to search for.
 * @return Pointer to the found macro, or NULL if not found.
 */
struct Macro *search_macro(struct MacroTable *mTable, char *name);

/**
 * @brief Concatenates two strings with memory allocation.
 * 
 * This function concatenates two strings and allocates memory for the resulting string.
 * 
 * @param str1 The first string.
 * @param str2 The second string.
 * @return Pointer to the concatenated string.
 */
char *strcatWithMalloc(const char *str1, const char *str2);

/**
 * @brief Opens a file and checks for errors.
 * 
 * This function opens a file with the specified mode and checks for errors.
 * 
 * @param filePath The path to the file.
 * @param mode The mode to open the file in.
 * @return Pointer to the opened file.
 */
FILE *openFileAndCheck(const char *filePath, const char *mode);

/**
 * @brief Processes a line of macro definition or usage.
 * 
 * This function processes a line of macro definition or usage and updates the current macro and macro table accordingly.
 * 
 * @param line_buffer The line buffer containing the line to process.
 * @param currentMacro Pointer to the current macro.
 * @param mTable Pointer to the macro table.
 * @return An integer indicating the type of line processed:
 *         0 for start of macro definition,
 *         1 for end of macro definition,
 *         2 for macro call,
 *         3 for normal line,
 *         -1 for error.
 */
int macro_line(char *line_buffer, struct Macro **currentMacro, struct MacroTable *mTable);

/**
 * @brief Prints the macro table.
 * 
 * This function prints the contents of the macro table for debugging purposes.
 * 
 * @param mTable Pointer to the macro table.
 */
void printMacroTable(struct MacroTable *mTable);

/**
 * @brief Preprocesses the assembly file.
 * 
 * This function preprocesses the assembly file by handling macro definitions and usages.
 * 
 * @param fName The name of the assembly file to preprocess.
 * @return Pointer to the name of the preprocessed file.
 */
char *preprocessor(char *fName);
#endif
