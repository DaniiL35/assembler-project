#include "firstpass.h"

#define MAX_LINE_LEN 100
#define WORD_LEN 100


struct Label lTable[100]; 

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
    char *temp_file_name;
    char mav_file_name[] = "mav.am";


    FILE *temp_file = openFileAndCheck("temp.am", "r");
    FILE *mav_file = openFileAndCheck(mav_file_name, "w"); 

    /* has label*/
    int has_label(char *label)
    
    while (fgets(Current_Line, MAX_LINE_LEN, temp_file) != NULL) {
        /* reset line */
        label[0] = '\0';
        command[0] = '\0';
        operand1[0] = '\0';
        operand2[0] = '\0';
        
        sscanf(Current_Line, "%s %s %s %s", label, command, operand1, operand2);

        /* rows that start with label */
        if (strcmp(label, "") != 0) {
            strcpy(current_label.name, label);
        }
    }

    fclose(temp_file);
    fclose(mav_file);

    return mav_file_name;
}