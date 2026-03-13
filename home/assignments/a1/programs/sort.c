#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reads all lines from stdin, stores them in a dynamic array
// Sorts them using merge sort then prints sorted lines to stdout

// Initial capacity for dynamic array of lines
#define INITIAL_CAPACITY 128

// Max buffer size for reading a single line from stdin
#define LINE_BUF_SIZE 4096

// Print error message to stderr and terminate program
static void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

// Allocate heap memory and copy a line into it
// This ensures each stored line has its own memory
static char *heap_copy_line(const char *src) {
    size_t n = strlen(src) + 1;
    char *dst = malloc(n);
    if (!dst) die("sort: malloc failed");
    memcpy(dst, src, n);
    return dst;
}

// Merge two sorted halves of arr[] into temp[]
// Left half = left..mid
// Right half: mid+1..right

static void merge(char **arr, char **temp,
                  size_t left, size_t mid, size_t right) {
    size_t i = left;
    size_t j = mid + 1;
    size_t k = left;

    while (i <= mid && j <= right) {
        if (strcmp(arr[i], arr[j]) <= 0) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (size_t x = left; x <= right; x++) {
        arr[x] = temp[x];
    }
}

// Recursive merge sort:
// Divide array into two halves
// Recursively sort each half
// Merge the sorted halves

static void merge_sort(char **arr, char **temp,
                       size_t left, size_t right) {
    if (left >= right) {
        return;
    }

    // Base case: 0 or 1 element is already sorted
    size_t mid = left + (right - left) / 2;

    merge_sort(arr, temp, left, mid);
    merge_sort(arr, temp, mid + 1, right);
    merge(arr, temp, left, mid, right);
}


// Program flow:
// Read all lines from stdin into a dynamically growing array
// Sort lines using merge sort
// Print sorted lines and free all allocated memory

int main(void) {
    size_t cap = INITIAL_CAPACITY;
    size_t len = 0;

    char **lines = malloc(cap * sizeof(char *));
    if (!lines) die("sort: malloc failed");

    char buf[LINE_BUF_SIZE];

    // Read all input lines into a dynamic array
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        
        // If array is full, double capacity
        if (len == cap) {
            cap *= 2;
            char **tmp = realloc(lines, cap * sizeof(char *));
            if (!tmp) {
                for (size_t i = 0; i < len; i++) free(lines[i]);
                free(lines);
                die("sort: realloc failed");
            }
            lines = tmp;
        }
        // Store a heap-allocated copy of the line and increment length
        lines[len++] = heap_copy_line(buf);
    }

    // Only sort if have more than one line
    if (len > 1) {
        // Temporary array used during merge operations
        char **temp = malloc(len * sizeof(char *));
        if (!temp) die("sort: malloc failed");
        merge_sort(lines, temp, 0, len - 1);
        free(temp);
    }

    // Output sorted lines to stdout and free each line's memory
    for (size_t i = 0; i < len; i++) {
        fputs(lines[i], stdout);
        free(lines[i]);
    }

    // Free dynamic array structure itself
    free(lines);
    return 0;
}