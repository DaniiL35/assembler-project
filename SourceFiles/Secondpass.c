/* Free all labels in the label table */
void free_labelTable(struct labelTable *labelTable) {
    int i;
    struct label *current_label;
    struct label *temp;

    for (i = 0; i < TABLE_SIZE; i++) {
        current_label = labelTable->table[i];
        while (current_label != NULL) {
            temp = current_label;
            current_label = current_label->next;
            free(temp);
        }
    }
}

