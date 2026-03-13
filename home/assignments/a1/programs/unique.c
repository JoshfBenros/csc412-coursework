#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Print only lines that differ from the immediately previous line
int main(void) {
    // Fixed-size buffers for the current line and the last line printed.
    char buffer[1024];
    char prev[1024];

    // First line is always printed
    int first_line = 1; 

    // Stream input line-by-line from stdin
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        
        // Print the first line, and remember it as the last-seen line
        if (first_line){
            fputs(buffer, stdout);
            strcpy(prev, buffer);
            first_line = 0;
        } 
        
        // For later lines print only if it differs from the previous printed line
        else if (strcmp(buffer, prev) != 0){
            fputs(buffer, stdout);
            strcpy(prev, buffer);
        }

        // If it's the same as prev, skip it
    }

    return 0;
}