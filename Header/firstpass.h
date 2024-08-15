#ifndef FIRSTPASS_H
#define FIRSTPASS_H

// Include any necessary libraries or headers
#include "utils.h"

// Declare any global variables or constants

// Declare struct definitions
struct Label {
    char name[WORD_LEN];
    int address;
    int extern;
    int entry;
    struct label *next;

};
// Declare function prototypes

#endif // FIRSTPASS_H