#include "firstpass.h"
#include "validation.h"

struct Label lTable[100];

/* label table structure */
struct labelTable {
    struct label *table[TABLE_SIZE];
};

/* Initialize the hash table */
void initlabelTable(struct labelTable *labelTable) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        labelTable->table[i] = NULL;
    }
}

/* Insert a label into the hash table */
void insertLabel(struct labelTable *labelTable, struct label *label) {
    unsigned int index = hash(label->Name);
    label->next = labelTable->table[index];
    labelTable->table[index] = label;
}

/* Find a label in the hash table */
struct label *search_label(struct labelTable *labelTable, char *name) {
    unsigned int index = hash(name);
    struct label *label = labelTable->table[index];
    while (label != NULL && strcmp(label->mName, name) != 0) {
        label = label->next;
    }
    return label;
}

/* ic calculate */
int ic_calculate(char *command, char *operand1, char *operand2) {
    int ic = 0;
    if (strcmp(command, "mov") == 0 || strcmp(command, "cmp") == 0 || strcmp(command, "add") == 0 || strcmp(command, "sub") == 0 || strcmp(command, "lea") == 0) {
        ic = 3;
    } else if (strcmp(command, "clr") == 0 || strcmp(command, "not") == 0 || strcmp(command, "inc") == 0 || strcmp(command, "dec") == 0 || strcmp(command, "jmp") == 0 || strcmp(command, "bne") == 0 || strcmp(command, "jsr") == 0 || strcmp(command, "red") == 0 || strcmp(command, "prn") == 0) {
        ic = 2;
    } else if (strcmp(command, "rts") == 0 || strcmp(command, "stop") == 0) {
        ic = 1;
    }
    if (strcmp(addressing_method(operand1), addressing_method(operand2)) == 0) {
        ic--;
    }
    return ic;
}

/* Function to count numbers in a string */
int count_numbers(char *str) {
    int count = 0;
    int i = 0;
    int len = strlen(str);

    while (i < len) {
        // Skip leading non-digit characters
        while (i < len && !isdigit(str[i]) && str[i] != '-' && str[i] != '+') {
            i++;
        }

        // Check if we have found a number
        if (i < len && (isdigit(str[i]) || ((str[i] == '-' || str[i] == '+') && i + 1 < len && isdigit(str[i + 1])))) {
            count++;
            // Skip the rest of the number
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
        // Skip leading non-alphabetic characters
        while (i < len && !isalpha(str[i])) {
            i++;
        }

        // Check if we have found an alphabetic sequence
        if (i < len && isalpha(str[i])) {
            count++;
            // Skip the rest of the alphabetic sequence
            while (i < len && isalpha(str[i])) {
                i++;
            }
        }
    }

    return count;
}

/* calculate DC */
int dc_calculate(char *command, char *operand1, char *operand2) {
    int dc = 0;
    if (strcmp(command, ".data") == 0) {
        dc = 1;
    } else if (strcmp(command, ".string") == 0) {
        dc = strlen(operand1) - 1;
    }
    return dc;
}

/* has label */
int has_label(char *label) {
    if (strcmp(label, "") != 0) {
        return true;
    }
    return false;
}

char *firstpass(char *fName) {
    int ic = 100;
    int dc = 0;
    char Current_Line[MAX_LINE_LEN];
    int line_num = 0;
    struct Label current_label = {0};
    char label[WORD_LEN];
    char command[WORD_LEN];
    char operand1[WORD_LEN];
    char operand2[WORD_LEN];
    int label_flag = 0;
    char *temp_file_name;
    char mav_file_name[] = "mav.am";

    FILE *temp_file = openFileAndCheck("temp.am", "r");
    FILE *mav_file = openFileAndCheck(mav_file_name, "w");

    while (fgets(Current_Line, MAX_LINE_LEN, temp_file) != NULL) {
        /* reset line */
        label[0] = '\0';
        command[0] = '\0';
        operand1[0] = '\0';
        operand2[0] = '\0';
        label_flag = 0;
        sscanf(Current_Line, "%s %s %s %s", label, command, operand1, operand2);

        /* check label flag */
        if (has_label(label))
            label_flag = 1;

        /* if dc */
        if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0) {

            /* check if command is .data or .string */
            if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0 && label_flag == 1) {
                /* check if the label is in the label table */
                struct label *current_label = search_label(&lTable, label);
                if (current_label != NULL) {
                    fprintf(stdout, "Error: label already exists\n");
                } else {
                    /* insert label into label table */
                    struct label *new_label = (struct label *)malloc(sizeof(struct label));
                    strcpy(new_label->Name, label);
                    new_label->Address = dc;
                    new_label->Extern = 0;
                    new_label->Entry = 0;
                    insertLabel(&lTable, new_label);
                    free(new_label);
                }
            }
            /* check if command is .extern */
            if (strcmp(command, ".extern") == 0) {
                /* check if the label is in the label table */
                struct label *current_label = search_label(&lTable, operand1);
                if (current_label != NULL) {
                    fprintf(stdout, "Error: Label already defined in this file; cannot be defined as extern.\n");
                } else {
                    /* insert label into label table */
                    struct label *new_label = (struct label *)malloc(sizeof(struct label));
                    strcpy(new_label->Name, operand1);
                    new_label->Address = 0;
                    new_label->Extern = 1;
                    new_label->Entry = 0;
                    insertLabel(&lTable, new_label);
                    free(new_label);
                }
            }
            /* check if command is .entry */
            if (strcmp(command, ".entry") == 0) {
                /* check if the label is in the label table */
                struct label *current_label = search_label(&lTable, operand1);
                if (current_label != NULL) {
                    current_label->Entry = 1;
                } else {
                    /* insert label into label table */
                    struct label *new_label = (struct label *)malloc(sizeof(struct label));
                    strcpy(new_label->Name, operand1);
                    new_label->Extern = 0;
                    new_label->Entry = 1;
                    insertLabel(&lTable, new_label);
                    free(new_label);
                }
            }
            /* increment dc */
            dc += dc_calculate(command, operand1, operand2);}

        /* instracion lines */
        if (strcmp(command, find_in_instruction_Table_table(command)) == 0 && label_flag == 1) {
            /* check if the label is in the label table */
            struct label *current_label = search_label(&lTable, label);
            if (current_label != NULL) {
                fprintf(stdout, "Error: label already exists\n");
            } else {
                /* insert label into label table */
                struct label *new_label = (struct label *)malloc(sizeof(struct label));
                strcpy(new_label->Name, label);
                new_label->Address = ic;
                new_label->Extern = 0;
                new_label->Entry = 0;
                insertLabel(&lTable, new_label);
                free(new_label);
            }
        }
    }

    /* loop end */
    fclose(temp_file);
    fclose(mav_file);

    return mav_file_name;
}
