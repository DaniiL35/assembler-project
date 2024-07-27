#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int is_valid_data(char **str) {
    while (**str != '\0') {
        skip_leading_sign(str);
        if (!isdigit(**str)) {
            return false;
        }
        strpbrk(*str, ",");
        skip_to_next_word(str);
        if (**str == ' ' || **str == '\t') {
            (*str)++;
        }
    }
    return true;
}

/* Function to skip the leading sign in a string */
void skip_leading_sign(char **str) {
    if ((*str)[0] == '+' || (*str)[0] == '-') {
        (*str)++;
    }
}/* Function to skip to the next word in a string */
void skip_to_next_word(char **str) {
    char *next_space = strpbrk(*str, " ");
    if (next_space != NULL) {
        *str = next_space + 1;
    }
}



int main