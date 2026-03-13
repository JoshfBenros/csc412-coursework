#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Allows the use of tolower()

// Filter that reads from stdin and writes to stdout
// Converts characters to lowercase

int main(void) {

    // Fixed size buffer allocated on the stack to store input read from stdin
    char buffer[1024];

    // fgets() reads from stdin - keeps going until EOF
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Convert each character in place to a lowercase until it hits the null terminator
        for (int i = 0; buffer[i] != '\0'; i++) {
            buffer[i] = (char)tolower((unsigned char)buffer[i]);
        }
        
        // Write the transformed buffer to stdout so it can be piped to the next program
        fputs(buffer, stdout);
    }

    return 0;
}
