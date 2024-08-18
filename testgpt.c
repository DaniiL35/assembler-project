#include <stdio.h>
#include <ctype.h>

int extract_number(const char* str) {
    int sign = 1;
    int num = 0;
    int found_number = 0;

    // Iterate through the string
    while (*str != '\0') {
        // Check for a negative sign
        if (*str == '-' && !found_number) {
            sign = -1;
        }
        
        // If it's a digit, start converting it to an integer
        if (isdigit(*str)) {
            num = num * 10 + (*str - '0');
            found_number = 1;
        } else if (found_number) {
            // If the number is found and a non-digit is encountered, stop
            break;
        }
        
        str++;
    }

    // Return the number with the correct sign
    return sign * num;
}

int main() {
    const char* str1 = "r3";
    const char* str2 = "#-6";
    const char* str3 = "#48";
    
    printf("Extracted number from \"%s\": %d\n", str1, extract_number(str1));  // Output: 3
    printf("Extracted number from \"%s\": %d\n", str2, extract_number(str2));  // Output: -6
    printf("Extracted number from \"%s\": %d\n", str3, extract_number(str3));  // Output: 48
    
    return 0;
}
