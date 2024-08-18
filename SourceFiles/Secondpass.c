#include "secondpass.h"
#include "math.h"
#include "firstpass.h"

#define BINARY_STR_LEN 16 /* 15 bits + 1 for null terminator */

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
void printLabelToExtern(struct labelTable *lTable, FILE *ext_file,char *operand1 , char *operand2,int *ic) {
    struct Label *label = NULL;
    int i;
    char extern_line[MAX_LINE_LEN];
    for (i = 0; i < TABLE_SIZE; i++) {
        label = lTable->table[i];
        while (label != NULL) {
            if(label->is_extern == 1){
                if(strcmp(operand1,label->name) == 0){
                    sprintf(extern_line, "%s %d\n", label->name, *ic);
                    fputs(extern_line, ext_file);
                }
                if(strcmp(operand2,label->name) == 0){
                    sprintf(extern_line, "%s %d\n", label->name, *ic);
                    fputs(extern_line, ext_file);
                }
            }
            label = label->next;
        }
    }

}

/* Function to convert binary string to unsigned int */
int binaryToOctal(const char *binary) {
    int octal = 0;
    int length = strlen(binary);
    int i, j;
    int temp;

    /* Convert binary to decimal */
    int decimal = 0;
    for (i = 0; i < length; i++) {
        if (binary[i] == '1') {
            decimal = (decimal << 1) | 1;
        } else if (binary[i] == '0') {
            decimal = decimal << 1;
        }
    }

    /* Convert decimal to octal */
    i = 1;
    while (decimal != 0) {
        temp = decimal % 8;
        octal += temp * i;
        decimal /= 8;
        i *= 10;
    }

    return octal;
}

void int_to_binary_str(int n, char *binaryStr, int bits) {
    int index = 0;
    int i;
    for (i = bits - 1; i >= 0; i--) {
        int bit = (n >> i) & 1;
        binaryStr[index++] = bit ? '1' : '0';
    }
    binaryStr[index] = '\0';  /* Null-terminate the string */
}

char* extractRegisterNumber(const char *operand) {
    static char binaryStr[4];  /* 3 bits + 1 for null terminator */
    int regNum;
    if (operand[0] == 'R' || operand[0] == 'r') {
        regNum = atoi(operand + 1);  /* Convert the number part to an integer */
        if (regNum >= 0 && regNum <= 7) {
            int_to_binary_str(regNum, binaryStr, 3);  /* Convert register number to binary string */
            return binaryStr;  /* Return the binary string */
        }
    }
    return NULL;  /* Error */
}

/* Maps instruction name to its opcode */
int get_opcode(char *command) {
    if (strcmp(command, "mov") == 0) return 0;
    if (strcmp(command, "cmp") == 0) return 1;
    if (strcmp(command, "add") == 0) return 2;
    if (strcmp(command, "sub") == 0) return 3;
    if (strcmp(command, "lea") == 0) return 4;
    if (strcmp(command, "clr") == 0) return 5;
    if (strcmp(command, "not") == 0) return 6;
    if (strcmp(command, "inc") == 0) return 7;
    if (strcmp(command, "dec") == 0) return 8;
    if (strcmp(command, "jmp") == 0) return 9;
    if (strcmp(command, "bne") == 0) return 10;
    if (strcmp(command, "red") == 0) return 11;
    if (strcmp(command, "prn") == 0) return 12;
    if (strcmp(command, "jsr") == 0) return 13;
    if (strcmp(command, "rts") == 0) return 14;
    if (strcmp(command, "stop") == 0) return 15;
    return -1;  /* Error or unknown command */
}

/* Maps operand to its addressing mode (just an example) */
int get_addressing_method(char *operand) {
    if (operand[0] == '#') return 0;       /* Immediate */
    if (operand[0] == '*') return 2;       /* Indirect */
    if (operand[0] == 'r') return 3;       /* Register direct */
    return 1;                              /* Direct */
}

/* Maps ARE (absolute, relocatable, external) */
int get_are_bits() {
    return 0;  /* Assuming absolute for simplicity */
}

/* Assembles the instruction into a binary string */
void assemble_instruction(int opcode, int src_addressing, int dest_addressing, int ARE, char *binary_instruction) {
    int instruction = 0;

    /* Shift and combine parts into the instruction */
    instruction |= (opcode & 0xF) << 11;              /* Opcode: 4 bits, shift to bits 11-14 */
    instruction |= (src_addressing & 0xF) << 7;       /* Source Addressing: 4 bits, shift to bits 7-10 */
    instruction |= (dest_addressing & 0xF) << 3;      /* Destination Addressing: 4 bits, shift to bits 3-6 */
    instruction |= (ARE & 0x7);                       /* ARE: 3 bits, in bits 0-2 */

    /* Convert the integer instruction to a binary string */
    int_to_binary_str(instruction, binary_instruction, 15); /* 15-bit binary instruction */
}

void check_label_table(struct labelTable *labelTable) {
    if (labelTable->count == 0) {
        printf("Label table is empty.\n");
    } else {
        printf("Label table has %d labels.\n", labelTable->count);
    }
}

/* Implementation of secondpass */
int* secondpass(char *validatedFileName, struct labelTable *labelTable, char *originalFileName) {
    FILE *input_file, *ob_file, *ent_file, *ext_file;
    char Current_Line[MAX_LINE_LEN];
    char label[WORD_LEN];
    char command[WORD_LEN];
    char operand1[WORD_LEN];
    char operand2[WORD_LEN];
    char *ob_file_name;  /* Output file name */
    char *ent_file_name;    /* Entry file name */
    char *ext_file_name;    /* External file name */
    char binary_instruction[16];  /* 15 bits + 1 for null terminator */
    char formatted_output[MAX_LINE_LEN];
    int opcode;
    int src_addressing;
    int dest_addressing;
    int are_bits;
    int prefix = 0;
    int ic = 100;
    char *src;
    char *dest;

    
    /* Check if labelTable is empty */
    check_label_table(labelTable);

    /* open and handle files */
    ob_file_name = strcatWithMalloc(originalFileName, ".ob");
    ent_file_name = strcatWithMalloc(originalFileName, ".ent");
    ext_file_name = strcatWithMalloc(originalFileName, ".ext");
    input_file = openFileAndCheck(validatedFileName, "r");
    ob_file = openFileAndCheck(ob_file_name, "a");
    ent_file = openFileAndCheck(ent_file_name, "w");
    ext_file = openFileAndCheck(ext_file_name, "w");

    /* Second pass processing */
    printf("Second pass started\n");
    while (fgets(Current_Line, MAX_LINE_LEN, input_file) != NULL) {
        /* reset line */
        label[0] = '0';
        command[0] = '0';
        operand1[0] = '0';
        operand2[0] = '0';
        

        sscanf(Current_Line, "%s %s %s %s", label, command, operand1, operand2);
        


        
        opcode = get_opcode(command);
        src_addressing = get_addressing_method(operand1);
        dest_addressing = get_addressing_method(operand2);
        are_bits = get_are_bits();

        

        /* Assemble the instruction into a binary string */
        assemble_instruction(opcode, src_addressing, dest_addressing, are_bits, binary_instruction);

        /* Write the binary string to the output file */
        sprintf(formatted_output, "%d%d %d", prefix, ic, binaryToOctal(binary_instruction));
        fputs(formatted_output, ob_file);
        fputs("\n", ob_file);  /* Newline for each instruction */
        ic++;

        /* write operand1 */
        if (strcmp(operand1, "0") != 0) {
            if (src_addressing == 2 || src_addressing == 3) {
                src = extractRegisterNumber(operand1);
                if (src != NULL) {
                    assemble_instruction(0, atoi(src), 0, 100, binary_instruction);
                }
            }
        }

        /* write operand2 */
        if (strcmp(operand2, "0") != 0) {
            if (dest_addressing == 2 || dest_addressing == 3) {
                dest = extractRegisterNumber(operand2);
                if (dest != NULL) {
                    assemble_instruction(0, atoi(dest), 0, 100, binary_instruction);
                }
            }
        }
    }

    /* write to ent */
    printLabelToEntry(labelTable, ent_file);
    
    /* write to ext */
    fputs("extern", ext_file);

    
    

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
    return 0;
}