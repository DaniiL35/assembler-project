#include "firstpass.h"
#include "validation.h"



/* Initialize the label table */
void initLabelTable(struct labelTable *lTable) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        lTable->table[i] = NULL;
    }
}


/* Insert a label into the hash table */
void insertLabel(struct labelTable *lTable, struct Label *label) {
    unsigned int index = hash(label->name);
    label->next = lTable->table[index];
    lTable->table[index] = label;
}

/* Find a label in the hash table */
struct Label *search_label(struct labelTable *lTable, char *name) {
    unsigned int index = hash(name);
    struct Label *label = lTable->table[index];
    while (label != NULL && strcmp(label->name, name) != 0) {
        label = label->next;
    }
    return label;
}
int command_calc(char *op1, char *op2){
    int i = 1;
    /*check addresing method*/
    if(strcmp(op1,"") != 0){
        i++;
    }
    if(strcmp(op2,"") != 0 && addressing_method(op1) != addressing_method(op2)){
        i++;
    }
    return i;
}



char* skipToDataOrString(char *line) {
    while (*line) {
        if (strncmp(line, ".data", 5) == 0 || strncmp(line, ".string", 7) == 0) {
            return line;
        }
        line++;
    }
    return NULL;  
}

int dc_calc(char *line) {
    if (strncmp(line, ".string", 7) == 0) {
        char *start = strchr(line, '"'); 
        if (start != NULL) {
            char *end = strchr(start + 1, '"'); 
            if (end != NULL) {
                return end - start; 
            }
        }
    } 
    else if (strncmp(line, ".data", 5) == 0) {
        int count = 0;
        char *token = strtok(line + 6, ","); 
        while (token != NULL) {
            count++;
            token = strtok(NULL, ",");
        }
        printf("in row %s there are %d numbers\n", line, count); /* Debugging */
        return count;
    }

    return 0; 
}


LabelDefResult label_def(char *line_buffer, struct Label **currentLabel, struct labelTable *lTable, int ic, int dc) {
    char label[MAX_LINE_LEN] = "";
    char command[MAX_LINE_LEN] = "";
    char op1[MAX_LINE_LEN] = "";
    char op2[MAX_LINE_LEN] = "";
    struct Label *newLabel = NULL;
    char *p1 = line_buffer;
    LabelDefResult result;
    char *line_ptr = line_buffer;




    sscanf(p1, " %s %s %s %s", label, command, op1, op2); 
    printf("label = %s, command = %s, op1 = %s, op2 = %s\n", label, command, op1, op2); /* Debugging */

    result.type = ic;
    result.number = -1;


    /* label definition */
    if (strcmp(label, "0") != 0) {
        newLabel = (struct Label *)malloc(sizeof(struct Label));
        if (newLabel == NULL) {
            exit(EXIT_FAILURE);
        }
        /* check if the label already exists */
        if (search_label(lTable, label) != NULL) {
            free(newLabel);
        } else {
            /* create a new label */
            strcpy(newLabel->name, label);
            if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0) {
                newLabel->address = ic+dc;
                result.type = dat;
                line_ptr = skipToDataOrString(line_ptr);
                result.number = dc_calc(line_ptr);
            } else if (strcmp(command, ".entry") == 0 || strcmp(command, ".extern") == 0) {
                newLabel->is_entry = 1;
            } else {
                newLabel->address = ic;
                result.type = com;
                result.number = command_calc(op1,op2); 
            }
            newLabel->is_extern = 0;
            newLabel->is_entry = 0;
            newLabel->next = NULL;
            insertLabel(lTable, newLabel);
            lTable->count++;
            *currentLabel = newLabel;
        }

    }
    else {
        if (strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0) {
                result.type = dat;
                line_ptr = skipToDataOrString(line_ptr);
                result.number = dc_calc(line_ptr);
        } else if (strcmp(command, ".entry") == 0 || strcmp(command, ".extern") == 0) {
            result.type = com;
            result.number = 0;
        } else {
            result.type = com;
            result.number = command_calc(op1,op2); 
        }
    }

    return result;
}

/* Function to preprocess the as file */
struct labelTable *firstpass(char *Vname, char *fName) {
    int ic = 100, dc = 0;
    char Current_Line[MAX_LINE_LEN];
    FILE *ob_file, *output_file;
    char *ob_file_name;
    int line = 1;
    char top_line[MAX_LINE_LEN];
    LabelDefResult line_type;

    struct Label *currentLabel = NULL;
    struct labelTable *lTable = (struct labelTable *)malloc(sizeof(struct labelTable));
    if (lTable == NULL) {
        exit(EXIT_FAILURE);
    }
    initLabelTable(lTable);
    ob_file_name = strcatWithMalloc(fName, ".ob");
    output_file = openFileAndCheck(Vname, "r");
    ob_file = openFileAndCheck(ob_file_name, "w");
    /* reading line by line from the as file */


    while (fgets(Current_Line, MAX_LINE_LEN, output_file) != NULL) { 
        line_type = label_def(Current_Line, &currentLabel, lTable, ic, dc);
        printf("reading line %s\n", Current_Line); /* Debugging */

        if(line_type.type == com){
            ic += line_type.number;
        } else if(line_type.type == dat){
            dc += line_type.number;
        }
        line++;
    }

    /* write the first line of the ob file */
    printf("First pass completed\n"); /* Debugging */
    printf("ic = %d, dc = %d\n", ic, dc); /* Debugging */
    sprintf(top_line, "\t%d %d\n", ic-100, dc);
    fputs(top_line, ob_file);

    /* free and close part */
    free(ob_file_name);
    fclose(ob_file);
    fclose(output_file); /* Close the input file as well */



    return lTable;
}
