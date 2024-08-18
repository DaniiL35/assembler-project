/**
 * @file main.h
 * @brief Header file for the main assembler program functions.
 *
 * This file provides declarations for functions used in the main assembler
 * program, including file existence checks and the main function for processing
 * assembly source files.
 */

#ifndef MAIN_H
#define MAIN_H

#include "utils.h"
#include "preprocessor.h"
#include "validation.h"
#include "firstpass.h"
#include "secondpass.h"

/**
 * @brief Check if a file with the ".as" extension exists.
 *
 * This function constructs the full file name by appending ".as" to the
 * provided base name and checks if the file exists in the file system.
 *
 * @param fName The base name of the file to check.
 *
 * @return 1 if the file exists, 0 otherwise.
 */
int checkFile(char *fName);

/**
 * @brief Entry point of the assembler program.
 *
 * This function processes command-line arguments to handle multiple assembly
 * source files. For each file, it performs existence checks, preprocessing,
 * validation, and both the first and second passes of the assembler. It
 * manages the file names and calls appropriate functions to carry out the
 * necessary processing.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 *
 * @return Exit status code (0 for success, 1 for failure).
 */
int main(int argc, char *argv[]);

#endif /* MAIN_H */
