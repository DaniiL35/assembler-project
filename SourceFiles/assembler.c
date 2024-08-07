#include "preprocessor.h"
#include "validation.h"



/* Main function */
int main() {
    char *fName = "ftest/test";
    preprocessor(fName);
    validation(fName);
    return 0;
}
