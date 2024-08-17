#include "firstpass.h"
#include "validation.h"

/* Initialize the hash table */
void initlabelTable(struct labelTable *labelTable) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        labelTable->table[i] = NULL;
    }
    labelTable->count = 0; 
}

/* Insert a label into the hash table */
void insertLabel(struct labelTable *labelTable, struct label *label) {
    unsigned int index = hash(label->label_Name);
    label->next = labelTable->table[index];
    labelTable->table[index] = label;
}

/* Find a label in the hash table */
struct label *search_label(struct labelTable *labelTable, char *name) {
    unsigned int index = hash(name);
    struct label *label = labelTable->table[index];
    while (label != NULL && strcmp(label->label_Name, name) != 0) {
        label = label->next;
    }
    return label;
}

/* Calculate IC */
int ic_calculate(char *command, char *operand1, char *operand2) {
    int ic = 0;

    if (strcmp(command, "mov") == 0 || strcmp(command, "cmp") == 0 || 
        strcmp(command, "add") == 0 || strcmp(command, "sub") == 0 || 
        strcmp(command, "lea") == 0) {
        if (strcmp(addressing_method(operand1), addressing_method(operand2)) == 0) {
            ic = 2;
        } else {
            ic = 3;
        }
    } else if (strcmp(command, "clr") == 0 || strcmp(command, "not") == 0 || 
               strcmp(command, "inc") == 0 || strcmp(command, "dec") == 0 || 
               strcmp(command, "jmp") == 0 || strcmp(command, "bne") == 0 || 
               strcmp(command, "jsr") == 0 || strcmp(command, "red") == 0 || 
               strcmp(command, "prn") == 0) {
        ic = 2;
    } else if (strcmp(command, "rts") == 0 || strcmp(command, "stop") == 0) {
        ic = 1;
    }

    return ic;
}

/* Function to count numbers in a string */
int count_numbers(char *str) {
    int count = 0;
    int i = 0;
    int len = strlen(str);

    while (i < len) {
        while (i < len && !isdigit(str[i]) && str[i] != '-' && str[i] != '+') {
            i++;
        }

        if (i < len && (isdigit(str[i]) || ((str[i] == '-' || str[i] == '+') && i + 1 < len && isdigit(str[i + 1])))) {
            count++;
            if (str[i] == '-' || str[i] == '+') {
                i++;
            }
            while (i < len && isdigit(str[i])) {
                i++;
            }
        }
    }

    return count;
}

/* Function to count alphabetic sequences in a string */
int count_letters(char *str) {
    int count = 0;
    int i = 0;
    int len = strlen(str);

    while (i < len) {
        while (i < len && !isalpha(str[i])) {
            i++;
        }

        if (i < len && isalpha(str[i])) {
            count++;
            while (i < len && isalpha(str[i])) {
                i++;
            }
        }
    }

    return count;
}

/* Calculate DC */
int dc_calculate(char *command, char *operand1, char *operand2) {
    int dc = 0;
    if (strcmp(command, ".data") == 0) {
        dc = 1 + count_numbers(operand1);
    } else if (strcmp(command, ".string") == 0) {
        dc = 1 + count_letters(operand1);
    }
    return dc;
}

/* Check if label exists */
int has_label(char *label) {
    return strcmp(label, "0") != 0;
}

/* First pass function */
struct labelTable *firstpass(char *vName, char *fName) {
    int ic = 100;
    int dc = 0;
    char Current_Line[MAX_LINE_LEN];
    int line_num = 0;
    struct labelTable *lTable;
    char label[WORD_LEN];
    char command[WORD_LEN];
    char operand1[WORD_LEN];
    char operand2[WORD_LEN];
    int label_flag = 0;
    char *ob_file_name;
    char first_line[50];
    FILE *ob_file;
    FILE *output_file;

    lTable = (struct labelTable *)malloc(sizeof(struct labelTable));
    if (lTable == NULL) {
        fprintf(stderr, "Error: Memory allocation for label table failed\n");
        return NULL;
    }

    ob_file_name = strcatWithMalloc(fName, ".ob");
    output_file = openFileAndCheck(vName, "r");
    ob_file = openFileAndCheck(ob_file_name, "w");

    if (ob_file == NULL || output_file == NULL) {
        free(lTable);
        return NULL;
    }

    initlabelTable(lTable);

    printf("First pass started\n");
    while (fgets(Current_Line, MAX_LINE_LEN, output_file) != NULL) {
        struct label *current_label;
        struct label *new_label;

        label[0] = '\0';
        command[0] = '\0';
        operand1[0] = '\0';
        operand2[0] = '\0';
        label_flag = 0;
        sscanf(Current_Line, "%s %s %s %s", label, command, operand1, operand2);

        if (strcmp(operand2, "dest") == 0 || strcmp(operand2, "source") == 0) {
            operand2[0] = '\0';
        }

        if (has_label(label)) {
            label_flag = 1;
        }

        if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0) {
            if (label_flag == 1) {
                current_label = search_label(lTable, label);
                if (current_label != NULL && current_label->is_extern == 0) {
                    fprintf(stdout, "Error: label already exists\n");
                } else {
                    new_label = (struct label *)malloc(sizeof(struct label));
                    if (new_label == NULL) {
                        fprintf(stderr, "Error: Memory allocation for label failed\n");
                        continue;
                    }
                    strcpy(new_label->label_Name, label);
                    new_label->address = dc;
                    new_label->is_extern = 0;
                    new_label->is_entry = 0;
                    lTable->count++;
                    insertLabel(lTable, new_label);
                }
            }
            dc += dc_calculate(command, operand1, operand2);
            line_num++;
            continue;
        }

        if (strcmp(command, ".extern") == 0 || strcmp(command, ".entry") == 0) {
            current_label = search_label(lTable, operand1);

            if (strcmp(command, ".extern") == 0) {
                if (current_label != NULL) {
                    fprintf(stdout, "Error: Label already defined in this file; cannot be defined as extern.\n");
                } else {
                    new_label = (struct label *)malloc(sizeof(struct label));
                    if (new_label == NULL) {
                        fprintf(stderr, "Error: Memory allocation for label failed\n");
                        continue;
                    }
                    strcpy(new_label->label_Name, operand1);
                    new_label->address = dc;
                    new_label->is_extern = 1;
                    new_label->is_entry = 0;
                    lTable->count++;
                    insertLabel(lTable, new_label);
                }
            }

            if (strcmp(command, ".entry") == 0) {
                if (current_label != NULL && current_label->is_extern == 0) {
                    current_label->is_entry = 1;
                } else {
                    new_label = (struct label *)malloc(sizeof(struct label));
                    if (new_label == NULL) {
                        fprintf(stderr, "Error: Memory allocation for label failed\n");
                        continue;
                    }
                    strcpy(new_label->label_Name, operand1);
                    new_label->is_extern = 0;
                    new_label->is_entry = 1;
                    insertLabel(lTable, new_label);
                }
            }
            line_num++;
            continue;
        }

        if (label_flag == 1) {
            current_label = search_label(lTable, label);
            if (current_label != NULL && current_label->is_extern == 0) {
                fprintf(stdout, "Error: label already exists\n");
            } else {
                new_label = (struct label *)malloc(sizeof(struct label));
                if (new_label == NULL) {
                    fprintf(stderr, "Error: Memory allocation for label failed\n");
                    continue;
                }
                strcpy(new_label->label_Name, label);
                new_label->address = ic;
                new_label->is_extern = 0;
                new_label->is_entry = 0;
                lTable->count++;
                insertLabel(lTable, new_label);
            }
        }
        ic += ic_calculate(command, operand1, operand2);
        line_num++;
    }
    sprintf(first_line, "\t%d %d\n", ic, dc);
    fputs(first_line, ob_file);

    fclose(output_file);
    fclose(ob_file);
    return lTable;
}