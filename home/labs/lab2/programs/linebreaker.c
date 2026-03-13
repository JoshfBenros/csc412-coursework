#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* linebreaker(const char* input) {
    // calculate the maximum possible len of the result
    size_t maxLen = strlen(input) + 1;

    char* result = (char*)malloc(maxLen); // allocate memory for the result
    if (result == NULL) {
        fprintf(stderr, "ERROR: memory allocation failed in linebreaker.\n");
        exit(1);
    }

    // loop throught the iput, if there is a space create a newline 
    size_t i = 0; // index for input
    size_t j = 0; // index for result
    while (input[i] != '\0'){
        if (input[i] == ' '){
            result[j] = '\n';
        } else {
            result[j] = input[i];
        }
        i++;
        j++;
    }
    result[j] = '\0';
    return result;
}

int main() {
    // creates a 1024 byte buffer to store data, will this be enough memory to store data?
    char buffer[1024];

    // Read data from stdin and write it to stdout (standard output)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // pass data from the buffer to a function convert spaces to linebreaks
        char* result = linebreaker(buffer);
        fputs(result, stdout);
        free(result); // releases the memory allocated
    }

    return 0;
}