#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "globals.h"

#ifndef __bool_true_false_are_defined
typedef enum {
    false,
    true
} bool;
#endif

/**
 * @brief Concatenates two strings, allocating memory dynamically.
 *
 * This function concatenates two input strings and returns a pointer
 * to the resulting string. Memory is allocated dynamically for the result.
 * 
 * @param str1 The first string.
 * @param str2 The second string.
 * @return A pointer to the concatenated string.
 */
char *strcatWithMalloc(const char *str1, const char *str2);


/**
 * @brief Concatenates two strings, allocating memory dynamically.
 *
 * This function concatenates two input strings and returns a pointer
 * to the resulting string. Memory is allocated dynamically for the result.
 * 
 * @param str1 The first string.
 * @param str2 The second string.
 * @return A pointer to the concatenated string.
 */
FILE *openFileAndCheck(const char *filePath, const char *mode);

/**
 * @brief Hashes a string using the djb2 algorithm.
 *
 * This function computes a hash for a given string using the djb2 algorithm.
 * 
 * @param str The string to hash.
 * @return The computed hash value modulo TABLE_SIZE.
 */
unsigned int hash(char *str);

#endif 