#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../fio/fio.h" // Local helper library (openFile) used to open files consistently

// sentence: print a text file, ensuring a final newline for the last line
int main(int argc, char **argv) {
    
    // Use command-line argument as filename; otherwise default to "unix_sentence.text".
    const char *filename = (argc > 1) ? argv[1] : "unix_sentence.text";
    
    // Fixed-size buffer to read file from disk
    char buffer[1024];

    // Open the file using the local fio library
    FILE *file = openFile(filename);
    if (file == NULL) {
        fprintf(stderr, "sentence: could not open file: %s\n", filename);
        return 1;
    }

    // Print each line, if the final line is missing '\n', append one
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            fputs(buffer, stdout);
        } else {
            printf("%s\n", buffer);
        }
    }

    fclose(file);

    return 0;
}