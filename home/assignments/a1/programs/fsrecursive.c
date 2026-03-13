#define _DEFAULT_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>


// Recursively traverses the Linux file system starting from the directory "fsrecursion_start.text"

// Task 1:
// 1) Prints the name of every directory encountered
// 2) Writes the same output to "fsrecursion_output.text"

// Task 2:
// 1) This output can later be piped into other Unix-style programs (lowercase, sort, unique, mismatch, etc.) to filter results

// Remove trailing newline from a string
// This ensures clean path strings read from the start file
static void rstrip_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[--n] = '\0';
    }
}

// Return the last component of a path
static const char *basename_simple(const char *path) {
    const char *slash = strrchr(path, '/');
    return slash ? slash + 1 : path;
}

// Print only the directory name
static void emit_dirname(const char *path, FILE *out) {
    const char *name = basename_simple(path);
    if (name[0] == '\0') name = "/"; // if path ends with '/' basename would be empty
    fprintf(out, "%s\n", name);
    printf("%s\n", name);
}

// Recursively walk through directories
static void walk_dirs(const char *path, FILE *out) {
    // Print this directory name first
    emit_dirname(path, out);

    // Get sorted directory entries
    // scandir allocates memory for namelist
    struct dirent **namelist = NULL;
    int n = scandir(path, &namelist, NULL, alphasort);
    if (n < 0) {
        // Can't open directory, stop
        return;
    }

    for (int i = 0; i < n; i++) {
        struct dirent *ent = namelist[i];
        const char *name = ent->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            free(ent);
            continue;
        }

        // Get sorted directory entries
        // scandir allocates memory for namelist
        char child[PATH_MAX];
        if (snprintf(child, sizeof(child), "%s/%s", path, name) >= (int)sizeof(child)) {
            free(ent);
            continue;
        }

        // Use lstat to detect links 
        struct stat st;
        if (lstat(child, &st) == 0) {
            // Recurse only into real directories (not symlink dirs)
            if (S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)) {
                walk_dirs(child, out);
            }
        }

        free(ent);
    }

    // Free memory allocated by scandir
    free(namelist);
}


// Program flow:
// 1) Read starting directory from "fsrecursion_start.text"
// 2) Default to "." if the file is empty
// 3) Open "fsrecursion_output.text" for writing
// 4) Recursively walk directory tree
// 5) Print names to both file and stdout

int main(void) {
    // Speed up output
    setvbuf(stdout, NULL, _IOFBF, 1 << 20);

    // Open file that contains starting directory path
    FILE *startf = fopen("fsrecursion_start.text", "r");
    if (!startf) {
        fprintf(stderr, "fsrecursive: could not open fsrecursion_start.text\n");
        return 1;
    }

    char start[PATH_MAX];
    if (!fgets(start, sizeof(start), startf)) {
        fprintf(stderr, "fsrecursive: fsrecursion_start.text is empty\n");
        fclose(startf);
        return 1;
    }
    fclose(startf);
    rstrip_newline(start);

    // If blank line in start file, use current directory
    if (start[0] == '\0') {
        strcpy(start, ".");
    }

    // Create/overwrite output file in programs directory
    FILE *out = fopen("fsrecursion_output.text", "w");
    if (!out) {
        fprintf(stderr, "fsrecursive: could not create fsrecursion_output.text\n");
        return 1;
    }
    // Increase output buffer size for performance
    setvbuf(out, NULL, _IOFBF, 1 << 20);

    walk_dirs(start, out);

    fclose(out);
    return 0;
}
