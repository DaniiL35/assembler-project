#ifndef REORDERING_H
#define REORDERING_H

#include "utils.h"

/**
 * @brief Reorders lines in a source file and writes them to a new file.
 * 
 * This function reads lines from the source file, classifies them into data lines,
 * entry/extern lines, and other lines, and writes them to the output file in the
 * required order.
 * 
 * @param source_file The name of the source file to read from.
 * @param new_name The name of the new file to write the reordered lines to.
 */
void reorder_lines(const char *source_file, char *new_name);

#endif 
