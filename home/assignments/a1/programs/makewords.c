#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Allows the use of isspace()

// Split input into words, one per line 
// Algorithm: single pass over the input
// *in_word tracks whether currently inside a word
char* makewords(const char* input, int *in_word) {
    // Worst case
    size_t maxLen = strlen(input) + 1;

    char* result = (char*)malloc(maxLen);
    if (result == NULL) {
        fprintf(stderr, "ERROR: memory allocation failed in makewords.\n");
        exit(1);
    }
    
    size_t j = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        unsigned char c = (unsigned char)input[i];

        // Copy non-whitespace characters directly into the output 
        // When a white space is hit, end the current word
        
        // Non-whitespace: we're inside a word; copy the character through
        if (!isspace(c)) {
            result[j++] = (char)c;
            *in_word = 1;
        } 

        // If word is finished emit exactly one newline
        // Any additional whitespace is ignored until another word/character is hit
        else {
            // Emit a newline at the end of a word, then skip extra whitespace
            if (*in_word) {
                result[j++] = '\n';
                *in_word = 0;
            }
        }

        // Safety check, never write past the allocated output buffer
        if (j >= maxLen - 1) {
            break;
        }
    }

    result[j] = '\0';

    return result;
}

int main(void) {
    // Fixed size buffer allocated on the stack to store input read from stdin
    char buffer[1024];

    // Keep track of whether we are inside a word across multiple fgets() reads
    int in_word = 0;

    // Read stdin in chunks, convert each chunk and stream the result to stdout
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        char* result = makewords(buffer, &in_word);
        fputs(result, stdout);
        free(result);
    }

    // If the input ends while in a word, terminate with a newline.
    if (in_word) {
        fputc('\n', stdout);
    }

    return 0;
}