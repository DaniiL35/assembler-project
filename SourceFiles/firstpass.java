#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define WORD_LEN 40
#define TABLE_SIZE 100

struct Label {
    char *name;
    char Extern;
    char Entry;
    int address;
};

/* Label table structure */
struct LabelTable {
    struct Label *table[TABLE_SIZE];
};

/* Hash function */
unsigned int hash(char *str) {
    unsigned int hash = 5381;
    int c;
    unsigned int index;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    index = hash % TABLE_SIZE;
    return index;
}

/* Initialize the hash table */
void initLabelTable(struct LabelTable *lTable) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        lTable->table[i] = NULL;
    }
}

/* Insert a label into the hash table */
void insertLabel(struct LabelTable *lTable, struct Label *label) {
    unsigned int index = hash(label->name);
    label->next = lTable->table[index];
    lTable->table[index] = label;
}

/* Find a label in the hash table */
struct Label *search_label(struct LabelTable *lTable, char *name) {
    unsigned int index = hash(name);
    struct Label *label = lTable->table[index];
    while (label != NULL && strcmp(label->name, name) != 0) {
        label = label->next;
    }
    return &label;
}



char * firstpass(char *fName) {
    int ic = 100;
    int dc = 0;
    char Current_Line[Max_LINE_LEN];
    int line_num = 0;
    struct current_label = {0};
    char label[WORD_LEN];
    char command[WORD_LEN];
    char operand1[WORD_LEN];
    char operand2[WORD_LEN];
    int i;
    char *temp_file_name;
    char mav_file_name = mav.am;

    FILE *temp_file = openFileAndCheck("temp.am", "r");
    FILE *mav_file = openFileAndCheck(mav_file_name, "w"); 
    
    while (fgets(Current_Line, Max_LINE_LEN, temp_file) != 0) 
        sscanf(Current_Line, "%s %s %s %s", label, command, operand1, operand2);
        if (label[0] == '0' && strcmp(command, ".string") != 0 && strcmp(command, ".data") != 0){
            
        }
        }

        


}