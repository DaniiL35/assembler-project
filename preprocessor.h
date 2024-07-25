#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>

/* Constants */
#define as_file_ext         ".as"
#define am_file_ext         ".am"
#define Max_macro_len_name  256
#define Max_LINE_LEN        80
#define Macro_start         "macr"
#define Macro_end           "endmacr"
#define TABLE_SIZE          1000
#define invalid_macro_table_size 30

/* Structure declarations */
struct Macro;
struct MacroTable;

/* Function declarations */
unsigned int hash(char *str);
void initMacroTable(struct MacroTable *mTable);
void insertMacro(struct MacroTable *mTable, struct Macro *macro);
struct Macro *search_macro(struct MacroTable *mTable, char *name);
char *strcatWithMalloc(const char *str1, const char *str2);
FILE *openFileAndCheck(const char *filePath, const char *mode);
int macro_line(char *line_buffer, struct Macro **currentMacro, struct MacroTable *mTable);
void printMacroTable(struct MacroTable *mTable);
char *preprocessor(char *fName);

#endif
