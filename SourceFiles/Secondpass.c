#include "secondpass.h"
#include "math.h"
#include "firstpass.h"
#include "validation.h"


#include <stdio.h>

/* Function to convert a 15-bit binary string to its octal representation */
char* binary_to_octal(const char* binary_str) {
    /* Declare variables at the start */
    static char octal_str[6];  /* Array to hold the resulting octal string (5 octal digits + null terminator) */
    int i, j;
    int binary_group;

    /* Initialize the index for the octal string */
    j = 0;

    /* Iterate over the binary string in groups of 3 bits */
    for (i = 0; i < 15; i += 3) {
        /* Convert each group of 3 binary digits to an integer (octal digit) */
        binary_group = (binary_str[i] - '0') * 4 + 
                       (binary_str[i+1] - '0') * 2 + 
                       (binary_str[i+2] - '0');
        
        /* Convert the integer to the corresponding octal digit */
        octal_str[j++] = binary_group + '0';
    }
    
    /* Null-terminate the octal string */
    octal_str[j] = '\0';

    return octal_str;
}

/* ascii to binary*/

/* Function to get the ASCII value of a character */
int getAsciiValue(char c) {
    return (int) c;
}

/* print label to entry file */
void printLabelToEntry(struct labelTable *lTable, FILE *ent_file) {
    struct Label *label = NULL;
    int i;
    char entry_line[MAX_LINE_LEN];
    for (i = 0; i < TABLE_SIZE; i++) {
        label = lTable->table[i];
        while (label != NULL) {
            if(label->is_entry == 1){
                sprintf(entry_line, "%s %d\n", label->name, label->address);
                fputs(entry_line, ent_file);
            }
            label = label->next;
        }
    }
}

/* print label to extern file */
void printLabelToExtern(struct labelTable *lTable, FILE *ext_file, char *operand1, char *operand2, int *ic) {
    struct Label *label = NULL;
    char extern_line[MAX_LINE_LEN];

    /* Check if operand1 is an extern label */
    label = search_label(lTable, operand1);
    if (label != NULL) {
        if (label->is_extern == 1) {
            sprintf(extern_line, "%s %d\n", label->name, *ic+1);
            fputs(extern_line, ext_file);
        }
    }

    /* Check if operand2 is an extern label */
    label = search_label(lTable, operand2);
    if (label != NULL) {
        if (label->is_extern == 1) {
            sprintf(extern_line, "%s %d\n", label->name, *ic+2);
            fputs(extern_line, ext_file);
        }
    }
}

char* trim_whitespace(char* str) {
    char* end;


    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = 0;

    return str;
}

/* function to decode string commands */
int decode_string(char *operand, int IC, FILE *ob_file) {
    int i;
    char formated_ob[MAX_LINE_LEN];
    char binaryString[16];
    data_bitField dbf = {0};
    for (i = 1; i < strlen(operand) - 1; i++) {
        dbf.data = getAsciiValue(operand[i]);
        toBinaryString(dbf.data, binaryString, 15);
        sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString));
        fputs(formated_ob, ob_file);
        fputs("\n", ob_file);
        IC++;
    }
    /* Print the ASCII code of '\0' at the end of each line */
    sprintf(formated_ob, "%04d %s", IC, binary_to_octal("000000000000000"));
    fputs(formated_ob, ob_file);
    fputs("\n", ob_file);
    return i;
}


/*function to decode data commands*/
int decode_data(char *operand, int IC, FILE *ob_file) {
    int i = 0;
    char formated_ob[MAX_LINE_LEN];
    char binaryString[16];
    data_bitField dbf = {0};
    char *token;

    operand = operand + 5;

    token = strtok(operand, ",");
    while (token != NULL) {
        token = trim_whitespace(token); 
        dbf.data = atoi(token);
        toBinaryString(dbf.data, binaryString, 15);
        sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString));
        fputs(formated_ob, ob_file);
        fputs("\n", ob_file);
        i++;
        IC++;
        token = strtok(NULL, ",");
    }
    return i;
}


int extract_number(char* str) {
    int sign = 1;
    int num = 0;
    int found_number = 0;

    /* Iterate through the string */
    while (*str != '\0') {
        /* Check for a negative sign */
        if (*str == '-' && !found_number) {
            sign = -1;
        }

        /* If it's a digit, start converting it to an integer */
        if (isdigit(*str)) {
            num = num * 10 + (*str - '0');
            found_number = 1;
        } else if (found_number) {
            /* If the number is found and a non-digit is encountered, stop */
            break;
        }

        str++;
    }

    /* Return the number with the correct sign */
    return sign * num;
}



/* get opcode from command*/
int getOpcode(char *command) {
    return instruction_Table[find_in_instruction_Table_table(command)].opcode;

}

int get_adressing(char *operand) {
    if (strstr(operand, "#")) { /* Immediate addressing */
        return 1;
    } else if (strstr(operand, "*r")) { /* Register addressing */
        return 4;
    } else if (strstr(operand, "r")) { /* Indirect addressing */
        return 8;
    } else { /* Direct addressing */
        return 2;
    }
}

/* get ARE from command */ /* need to work on logic */
int get_ARE(char *operand) {
    return 1;

}

unsigned int combineBitField(BitField bf) {
    return (bf.opcode << 11) | (bf.src << 7) | (bf.dest << 3) | bf.are;
}

unsigned int combineRegBitField(reg_bitField op_bf) {
    return (op_bf.free << 9) | (op_bf.src << 6) | (op_bf.dest << 3) | op_bf.are;
}

unsigned int combineLabelBitField(label_bitField op_bf) {
    return (op_bf.dest << 3) | op_bf.are;
}

void toBinaryString(unsigned int num, char *str, int length) {
    int i;
    str[length] = '\0';
    for (i = length - 1; i >= 0; i--) {
        str[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }
}

int number_of_operands(char * cmd){
return instruction_Table[find_in_instruction_Table_table(cmd)].num_of_operands;
}

/* Function to convert a command to binary */
int commandToBinary(char *line, struct labelTable *labelTable, FILE *ob_file,FILE *ext_file, int IC) {
    char label[MAX_LINE_LEN], command[MAX_LINE_LEN], operand1[MAX_LINE_LEN], operand2[MAX_LINE_LEN];
    BitField bf = {0}; 
    unsigned int combined;
    char binaryString[16]; 
    char formated_ob[MAX_LINE_LEN];
    int num = 0;
    int count = 0;
    char *line_ptr = line;
    sscanf(line, "%s %s %s %s", label, command, operand1, operand2);

    
    printLabelToExtern(labelTable, ext_file, operand1, operand2, &IC);

    if(strcmp(command, ".data") == 0 || strcmp(command, ".string") == 0){
        num = 3;
    } else if(strcmp(command, ".entry") == 0 || strcmp(command, ".extern") == 0){
        num = 4;
    } else {
        num = number_of_operands(command);
    }

    switch (num) {
        case 0: /* for codes withot operands*/
            bf.opcode = getOpcode(command);
            bf.src = 0;
            bf.dest = 0;
            bf.are = 4;
            break;
        case 1: /* for codes with one operand */
            bf.opcode = getOpcode(command);
            bf.src = 0;
            bf.dest = get_adressing(operand1);
            bf.are = 4;
            break;
        case 2: /* for codes with two operands */
            bf.opcode = getOpcode(command);
            bf.src = get_adressing(operand1);
            bf.dest = get_adressing(operand2);
            bf.are = 4;
            break;
        case 3: /* for .data or .string*/
            /* handle .string*/
            if(strcmp(command, ".string") == 0){
                count += decode_string(operand1,IC, ob_file);
                return count;
            }else{
            /* handle .data*/
            line_ptr = skipToDataOrString(line_ptr);
            count += decode_data(line_ptr,IC, ob_file);
            return count;
            }
            break;
        case 4: /* for .entry or .extern*/
            if(strcmp(command, ".entry") == 0){
                return 0;
            }
            return 0;
            break;
        default:
            /* handle error */
            break;
    }
    /* print to ob file */
    combined = combineBitField(bf);
    toBinaryString(combined, binaryString, 15);
    sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString));
    fputs(formated_ob, ob_file);
    fputs("\n", ob_file);
    count++;

    /* process ops */
        switch (num) {
        case 0: /* for codes withot operands*/
            break;
        case 1: /* for codes with one operand */
            opToBinary(operand1, labelTable, ob_file, IC+1);
            count++;
            break;
        case 2: /* for codes with two operands */
            count += opToBinaryDouble(operand1, operand2, labelTable, ob_file, IC+1);
            break;
        case 3: /* for .data or .string*/
        default:
            /* handle error */
            break;
    }




return count;
}

    void opToBinary(char *op, struct labelTable *labelTable, FILE *ob_file, int IC) {
    reg_bitField rbf = {0};
    label_bitField lbf = {0};
    unsigned int combined;
    char binaryString[16];
    char formated_ob[MAX_LINE_LEN];
    struct Label *label = NULL;

    if (search_label(labelTable, op) != NULL) {
        label = search_label(labelTable, op);
        if( label->is_extern == 1){
            lbf.dest = 0;
            lbf.are = 1;
        }else{
            lbf.dest = label->address;
            lbf.are = 2;
        }
        combined = combineLabelBitField(lbf);
        toBinaryString(combined, binaryString, 15);
        sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString));
        fputs(formated_ob, ob_file);
        fputs("\n", ob_file);
        

    }else if(get_adressing(op) == 1){
            lbf.dest = extract_number(op);
            lbf.are = 4;
            combined = combineLabelBitField(lbf);
            toBinaryString(combined, binaryString, 15);
            sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString));
            fputs(formated_ob, ob_file);
            fputs("\n", ob_file);
    } else {
        rbf.free = 0;
        rbf.src = 0;
        rbf.dest = extract_number(op);
        rbf.are = 4;
        combined = combineRegBitField(rbf);
        toBinaryString(combined, binaryString, 15);
        sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString));
        fputs(formated_ob, ob_file);
        fputs("\n", ob_file);
}
}
int opToBinaryDouble(char *op1, char *op2, struct labelTable *labelTable, FILE *ob_file, int IC) {
    reg_bitField rbf1 = {0};
    label_bitField lbf = {0};
    unsigned int combined;
    char binaryString1[16];
    char binaryString2[16];
    char formated_ob[MAX_LINE_LEN];
    struct Label *label = NULL;

    


if ((strcmp(addressing_method(op1), "2") == 0 || strcmp(addressing_method(op1), "3") == 0) &&
    (strcmp(addressing_method(op2), "2") == 0 || strcmp(addressing_method(op2), "3") == 0)) {
        rbf1.free = 0;
        rbf1.src = extract_number(op1);
        rbf1.dest = extract_number(op2);
        rbf1.are = 4;
        combined = combineRegBitField(rbf1);
        toBinaryString(combined, binaryString1, 15);
        sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString1));
        fputs(formated_ob, ob_file);
        fputs("\n", ob_file);
        return 1;
    } else{     /* ops are mixed */
        /* process op1*/
        if (search_label(labelTable, op1) != NULL) {
            label = search_label(labelTable, op1);
            if( label->is_extern == 1){
            lbf.dest = 0;
            lbf.are = 1;
        }else{
            lbf.dest = label->address;
            lbf.are = 2;
        }
            combined = combineLabelBitField(lbf);
            toBinaryString(combined, binaryString1, 15);
            sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString1));
            fputs(formated_ob, ob_file);
            fputs("\n", ob_file);
       }else if(addressing_method(op1) == 0){
            lbf.dest = extract_number(op1);
            lbf.are = 4;
            combined = combineLabelBitField(lbf);
            toBinaryString(combined, binaryString1, 15);
            sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString1));
            fputs(formated_ob, ob_file);
            fputs("\n", ob_file);
        } else {
            rbf1.free = 0;
            rbf1.src = extract_number(op1);
            rbf1.dest = 0;
            rbf1.are = 4;
            combined = combineRegBitField(rbf1);
            toBinaryString(combined, binaryString1, 15);
            sprintf(formated_ob, "%04d %s", IC, binary_to_octal(binaryString1));
            fputs(formated_ob, ob_file);
            fputs("\n", ob_file);
        }
    }
    /* process op2*/
    if (search_label(labelTable, op2) != NULL) {
        label = search_label(labelTable, op2);
        if( label->is_extern == 1){
            lbf.dest = 0;
            lbf.are = 1;
        }else{
            lbf.dest = label->address;
            lbf.are = 2;
        }
        combined = combineLabelBitField(lbf);
        toBinaryString(combined, binaryString2, 15);
        sprintf(formated_ob, "%04d %s", IC+1, binary_to_octal(binaryString2));
        fputs(formated_ob, ob_file);
        fputs("\n", ob_file);
    } else if(strcmp(addressing_method(op2), "0") == 0){
        lbf.dest = extract_number(op2);
        lbf.are = 4;
        combined = combineLabelBitField(lbf);
        toBinaryString(combined, binaryString2, 15);
        sprintf(formated_ob, "%04d %s", IC+1, binary_to_octal(binaryString2));
        fputs(formated_ob, ob_file);
        fputs("\n", ob_file);
    } else {
        rbf1.free = 0;
        rbf1.src = 0;
        rbf1.dest = extract_number(op2);
        rbf1.are = 4;
        combined = combineRegBitField(rbf1);
        toBinaryString(combined, binaryString2, 15);
        sprintf(formated_ob, "%04d %s", IC+1, binary_to_octal(binaryString2));
        fputs(formated_ob, ob_file);
        fputs("\n", ob_file);
    }
    return 2;
    }





/* Implementation of secondpass */
int* secondpass(char *validatedFileName, struct labelTable *labelTable, char *originalFileName) {
    FILE *input_file, *ob_file, *ent_file, *ext_file;
    char Current_Line[MAX_LINE_LEN];
    char *ob_file_name;  /* Output file name */
    char *ent_file_name;    /* Entry file name */
    char *ext_file_name;    /* External file name */
    int IC = 100; /* Instruction Counter */

    


    /* open and handle files */
    ob_file_name = strcatWithMalloc(originalFileName, ".ob");
    ent_file_name = strcatWithMalloc(originalFileName, ".ent");
    ext_file_name = strcatWithMalloc(originalFileName, ".ext");
    input_file = openFileAndCheck(validatedFileName, "r");
    ob_file = openFileAndCheck(ob_file_name, "a");
    ent_file = openFileAndCheck(ent_file_name, "w");
    ext_file = openFileAndCheck(ext_file_name, "w");

    /* Second pass processing */
    while (fgets(Current_Line, MAX_LINE_LEN, input_file) != NULL) {
        /* Check if the line contains ".extern" or ".entry" */
        if (strstr(Current_Line, ".extern") != NULL || strstr(Current_Line, ".entry") != NULL) {
            continue; 
        }
    /* codding the command line and print to the .ob*/
    IC += commandToBinary(Current_Line, labelTable, ob_file, ext_file, IC);
    

    }

    /* write to ent */ 
    printLabelToEntry(labelTable, ent_file);
    

    
    

    /*free seciton*/
    free(ob_file_name);
    free(ent_file_name);
    free(ext_file_name);
    free(labelTable);
    


    /* Close the files */
    fclose(input_file);
    fclose(ob_file);
    fclose(ent_file);
    fclose(ext_file);
    remove(validatedFileName);
    return 0;
}

