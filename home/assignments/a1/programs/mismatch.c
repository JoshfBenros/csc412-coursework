#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// number of buckets in the hash table (size helps reduce collisions)
#define TABLE_SIZE 10007

// Read candidate words from stdin and print only those not found in a dictionary file
// Approach: load the dictionary into a hash table (separate chaining), then stream stdin 

// Hash table node (linked list) for collision handling (separate chaining)
typedef struct Node {
    char *word;
    struct Node *next;
} Node;

// Hash table buckets. Each bucket is a linked list of dictionary words that share the same hash index
static Node *table[TABLE_SIZE];

// Maps a word to an unsigned long hash value 
static unsigned long hash_str(const char *s) {
    unsigned long h = 5381;
    unsigned char c;
    
    // Bit shift 
    while ((c = (unsigned char)*s++) != 0) {
        h = ((h << 5) + h) + c;
    }
    return h;
}

// Remove trailing '\n' from fgets() so comparisons use clean strings 
static void rstrip_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') {
        s[n - 1] = '\0';
    }
}

// Insert a dictionary word into the hash table
// compute bucket index -> allocate node + copy string -> push onto bucket's linked list
static void dict_insert(const char *word) {
    
    // Pick a bucket based on the hash
    unsigned long h = hash_str(word) % TABLE_SIZE;
    Node *node = malloc(sizeof(*node));
    if (!node) {
        fprintf(stderr, "mismatch: malloc failed\n");
        exit(1);
    }
    size_t n = strlen(word) + 1;
    
    // Insert at head of bucket list
    node->word = malloc(n);
    if (!node->word) {
        fprintf(stderr, "mismatch: malloc failed\n");
        exit(1);
    }
    memcpy(node->word, word, n);
    node->next = table[h];
    table[h] = node;
}

// Check whether a word exists in the dictionary hash table by scanning its bucket list
static int dict_contains(const char *word) {
    unsigned long h = hash_str(word) % TABLE_SIZE;
    
    // Only scan the one bucket for this word
    for (Node *node = table[h]; node != NULL; node = node->next) {
        if (strcmp(node->word, word) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    char word[1024];
    char dict_word[1024];

    // Dictionary file path comes from argv[1] - default to "unix_dict.text"
    const char *filename = (argc > 1) ? argv[1] : "unix_dict.text";
    FILE *dict = fopen(filename, "r");
    if (dict == NULL) {
        fprintf(stderr, "mismatch: could not open dictionary file: %s\n", filename);
        return 1;
    }

    // Load dictionary words into a hash table for quick lookup
    // The dictionary comes from a file. Candidate words come from stdin (pipeline)
    while (fgets(dict_word, sizeof(dict_word), dict) != NULL) {
        rstrip_newline(dict_word);
        
        // Skip empty lines after stripping newline
        if (dict_word[0] != '\0') {
            dict_insert(dict_word);
        }
    }
    fclose(dict);

    // Stream stdin and print only words missing from the dictionary
    while (fgets(word, sizeof(word), stdin) != NULL) {
        rstrip_newline(word);
        
        // If the word is not in the dictionary, emit it to stdout
        if (!dict_contains(word)) {
            printf("%s\n", word);
        }
    }

    return 0;
}