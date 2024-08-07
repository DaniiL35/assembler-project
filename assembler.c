#include "preprocessor.h"
#include "validation.h"



/* Main function */
int main() {
    char *fName = "test";
    preprocessor(fName);
    validation(fName);
    return 0;
}
