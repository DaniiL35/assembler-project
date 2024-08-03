


struct label {
    char *name;
    int address;
    int DC;
    int IC;
    struct label *next;
};


int find_label_in_list (char *label, struct label *head) {
    struct label *current = head;
    while (current != NULL) {
        if (strcmp(current->name, label) == 0) {
            return current->address;
        }
        current = current->next;
    }
    return -1;
}

void add_label_to_list (char *label, int address, int DC, int IC, struct label **head) {
    struct label *new_label = (struct label *) malloc(sizeof(struct label));
    new_label->name = (char *) malloc(strlen(label) + 1);
    strcpy(new_label->name, label);
    new_label->address = address;
    new_label->DC = DC;
    new_label->IC = IC;
    new_label->next = *head;
    *head = new_label;
}








char *first_pass(char *fname) {
 int line_counter = 0;
    char Current_Line[Max_LINE_LEN];
    char *line_ptr;
    char *addres_mode;
    char instruction_temp[4];
    char *current_word = (char *)malloc(Max_LINE_LEN * sizeof(char));
    char *am_file_name = strcatWithMalloc(fName, am_file_ext);
    FILE *am_file = openFileAndCheck(am_file_name, "r");
    FILE *temp_file = openFileAndCheck("temp.am", "w");
    printf("Reading started\n"); /* testing only */

    /* reading line by line from the am file */
    while (fgets(Current_Line, Max_LINE_LEN, am_file) != 0) {

}
}