#include "firstpass.h"
#include "validation.h"

/* Initialize the hash table */
void initlabelTable(struct labelTable *labelTable) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        labelTable->table[i] = NULL;
    }
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

/* ic calculate */
int ic_calculate(char *command, char *operand1, char *operand2) {
    int ic = 0;


    if (strcmp(command, "mov") == 0 || strcmp(command, "cmp") == 0 || 
        strcmp(command, "add") == 0 || strcmp(command, "sub") == 0 || 
        strcmp(command, "lea") == 0) {
            /* Check if the operands are of the same addressing method */
            if (strcmp(addressing_method(operand1), addressing_method(operand2)) == 0) {
                printf("operands are of the same addressing method\n"); /* debug */
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

    /* Check if the operands are of the same addressing method */
    printf("operand1 = %s\n", operand1); /* debug */
    printf("operand1 addressing method = %s\n", addressing_method(operand1)); /* debug */
    printf("operand2 = %s\n", operand2); /* debug */
    printf("operand2 addressing method = %s\n", addressing_method(operand2)); /* debug */
    

    return ic;
}


/* Function to count numbers in a string */
int count_numbers(char *str) {
    int count = 0;
    int i = 0;
    int len = strlen(str);

    while (i < len) {
        /* Skip leading non-digit characters */
        while (i < len && !isdigit(str[i]) && str[i] != '-' && str[i] != '+') {
            i++;
        }

        /* Check if we have found a number */
        if (i < len && (isdigit(str[i]) || ((str[i] == '-' || str[i] == '+') && i + 1 < len && isdigit(str[i + 1])))) {
            count++;
            /* Skip the rest of the number */
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
        /* Skip leading non-alphabetic characters */
        while (i < len && !isalpha(str[i])) {
            i++;
        }

        /* Check if we have found an alphabetic sequence */
        if (i < len && isalpha(str[i])) {
            count++;
            /* Skip the rest of the alphabetic sequence */
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
        dc = 1+count_numbers(operand1);
    } else if (strcmp(command, ".string") == 0) {
        printf("operand1 in .data = %s\n", operand1); /* debug */
        dc = 1+count_letters(operand1);
    }
    return dc;
}

/* has label */
int has_label(char *label) {
    if (strcmp(label, "0") != 0) {
        return true;
    }
    return false;
}

void * firstpass(char *fName) {
    int ic = 100;
    int dc = 0;
    char Current_Line[MAX_LINE_LEN];
    int line_num = 0;
    char *line_ptr;
    struct labelTable lTable;
    char label[WORD_LEN];
    char command[WORD_LEN];
    char operand1[WORD_LEN];
    char operand2[WORD_LEN];
    int label_flag = 0;

    FILE *output_file;
    FILE *mav_file;
    mav_file = openFileAndCheck("mav_file", "w");
    output_file = openFileAndCheck(fName, "r");

    initlabelTable(&lTable);




    /* firstpass started */
    printf("First pass started\n");
    while (fgets(Current_Line, MAX_LINE_LEN, output_file) != NULL) {
        /* reset line */
        label[0] = '\0';
        command[0] = '\0';
        operand1[0] = '\0';
        operand2[0] = '\0';
        label_flag = 0;
        sscanf(Current_Line, "%s %s %s %s", label, command, operand1, operand2);  

        /* check label flag */
        if (has_label(label)){
            printf("label found\n %s\n", label);
            label_flag = 1;
        }
        /*data type command*/
        if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0) {

            /* check if command is .data or .string */
            if ((strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0) && label_flag == 1) {
                /* check if the label is in the label table */
                struct label *current_label = search_label(&lTable, label);
                if (current_label != NULL && current_label->is_extern == 0) {
                    fprintf(stdout, "Error: label already exists\n");
                } else {
                    /* insert label into label table */
                    struct label *new_label = (struct label *)malloc(sizeof(struct label));
                    strcpy(new_label->label_Name, label);
                    new_label->address = dc;
                    new_label->is_extern = 0;
                    new_label->is_entry = 0;
                    insertLabel(&lTable, new_label);
                    printf("label dc address = %d\n", dc); /* debug */
                }
            }
            dc += dc_calculate(command, operand1, operand2);
            continue;
        }

        /* Handle .extern or .entry command */
        if (strcmp(command, ".extern") == 0 || strcmp(command, ".entry") == 0) {

            struct label *current_label = search_label(&lTable, operand1);

            /* Handle .extern command */
            if (strcmp(command, ".extern") == 0) {
                if (current_label != NULL) {
                    fprintf(stdout, "Error: Label already defined in this file; cannot be defined as extern.\n");
                } else {
                    struct label *new_label = (struct label *)malloc(sizeof(struct label));
                    strcpy(new_label->label_Name, operand1);
                    new_label->address = dc;
                    new_label->is_extern = 1;
                    new_label->is_entry = 0;
                    insertLabel(&lTable, new_label);
                }
            }

            /* Handle .entry command */
            if (strcmp(command, ".entry") == 0) {
                if (current_label != NULL && current_label->is_extern == 0) {
                    current_label->is_entry = 1;
                } else {
                    struct label *new_label = (struct label *)malloc(sizeof(struct label));
                    strcpy(new_label->label_Name, operand1);
                    new_label->is_extern = 0;
                    new_label->is_entry = 1;
                    insertLabel(&lTable, new_label);
                }
            }
            continue;
        }
        /*handale instacion commands*/
        if(label_flag == 1){
            struct label *current_label = search_label(&lTable, label);
            if (current_label != NULL && current_label->is_extern == 0) {
                fprintf(stdout, "Error: label already exists\n");
            } else {
                struct label *new_label = (struct label *)malloc(sizeof(struct label));
                strcpy(new_label->label_Name, label);
                new_label->address = ic;
                new_label->is_extern = 0;
                new_label->is_entry = 0;
                insertLabel(&lTable, new_label);
                printf("label IC address = %d\n", ic); /* debug */
            }
        }
        ic += ic_calculate(command, operand1, operand2);
        line_num++;
        continue;


    }
    printf("ic = %d\n", ic); /* debug */
    printf("dc = %d\n", dc); /* debug */
    printf("line %d\n", line_num); /* debug */
    printf("First pass ended\n");

    /* loop end */

    fclose(output_file);
    fclose(mav_file);
    return 0;
}