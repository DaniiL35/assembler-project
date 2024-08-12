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

char *strcatWithMalloc(const char *str1, const char *str2);
FILE *openFileAndCheck(const char *filePath, const char *mode);
unsigned int hash(char *str);

#endif 