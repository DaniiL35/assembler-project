
#include "reordering.h"

void reorder_lines(const char *source_file, char * new_name) {
    char output_file[MAX_LINE_LEN];
    FILE *source;
    FILE *output;
    char line[MAX_LINE_LEN];
    char **data_lines = NULL;
    char **other_lines = NULL;
    int data_count = 0;
    int other_count = 0;
    int i;
    strcpy(output_file, new_name);


    source = fopen(source_file, "r");
    output = fopen(output_file, "w");

    if (source == NULL || output == NULL) {
        perror("Error opening file");
        exit(1);
    }

    /* Read lines from source file and classify them */
    while (fgets(line, sizeof(line), source)) {
        if (strstr(line, ".data") != NULL || strstr(line, ".string") != NULL) {
            data_lines = realloc(data_lines, (data_count + 1) * sizeof(char *));
            data_lines[data_count] = malloc(strlen(line) + 1); /* Allocate memory */
            strcpy(data_lines[data_count], line); /* Copy the line */
            data_count++;
        } else {
            other_lines = realloc(other_lines, (other_count + 1) * sizeof(char *));
            other_lines[other_count] = malloc(strlen(line) + 1); /* Allocate memory */
            strcpy(other_lines[other_count], line); /* Copy the line */
            other_count++;
        }
    }

    for (i = 0; i < other_count; i++) {
        fputs(other_lines[i], output);
        free(other_lines[i]);
    }
    free(other_lines);

    for (i = 0; i < data_count; i++) {
        fputs(data_lines[i], output);
        free(data_lines[i]);
    }
    free(data_lines);

    fclose(source);
    fclose(output);

 printf("Reordering done\n"); /* Debug*/   
}
