#ifndef REORDERING_H
#define REORDERING_H

#include "utils.h"

/**
 * @brief Reorders lines from a source file into a new file.
 *
 * This function reads lines from the source file, classifies them into
 * different categories (.data, .string, .entry, .extern), and writes them
 * to a new file in the order: other lines, .data/.string lines, .entry/.extern lines.
 * 
 * @param source_file The path to the source file.
 * @param new_name The path for the new file with reordered lines.
 */
void reorder_lines(const char *source_file, char *new_name);

#endif 
