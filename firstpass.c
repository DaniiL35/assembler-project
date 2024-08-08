struct Label {
    char *name;
    int address;
    char Extern;
    char Entry;
};








char * firstpass(char *fName) {
    int ic = 100;
    int dc = 0;
    char Current_Line[Max_LINE_LEN];
    FILE *temp_file;
    char label[];
    char command[];
    char operand1[];
    char operand2[];



    while (fgets(Current_Line, Max_LINE_LEN, temp_file) != 0) 
        sscanf(Current_Line, "%s %s %s %s", label, command, operand1, operand2);
        if (label[0] == ';') {

        }
        if(command == ".data" || command == ".string") {
            
        }


}