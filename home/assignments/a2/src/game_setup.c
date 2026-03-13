#include "game_setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for isdigit() to safely check numeric characters 

// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p) {
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;

    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep) {
    // TODO: implement!
    
    // null check 
    if (!cells_p || !width_p || !height_p){
        return INIT_ERR_BAD_CHAR;
    }

    // Ensure outputs are always in a safe state, even if initialization fails.
    *cells_p = NULL;
    *width_p = 0;
    *height_p = 0;

    // Reset global game state for fresh run 
    g_game_over = 0;
    g_score = 0;

    if (snake_p) {
        snake_p->positions = NULL;
        snake_p->length = 0;
        snake_p->capacity = 0;
        snake_p->dx = 1;
        snake_p->dy = 0;
        snake_p->initialized = 0;
    }

    // Build the board first
    enum board_init_status st;
    if (board_rep == NULL) {
        st = initialize_default_board(cells_p, width_p, height_p);
    } else {
        st = decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
    }

    // If board setup failed, stop
    if (st != INIT_SUCCESS) {
        return st;
    }

    // Ensure one pience of food exists at game start
    place_food(*cells_p, *width_p, *height_p);

    return INIT_SUCCESS;
}

/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */

 // Helper function 
 // Conver a compressed board character into its corresponding cell flag 
 // Returns -1 if the character is invalid 
 static int cell_from_char(char c) {
  switch (c) {
    case 'W': return FLAG_WALL;
    case 'S': return FLAG_SNAKE;
    case 'F': return FLAG_FOOD;
    case 'E': return FLAG_PLAIN_CELL;
    default:  return -1;
  }
}

// Decompress a run length encoded board string
// B<height>x<width>|<row_data>
// Allocates and fills the cells array and sets width/height

enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed) {
    // TODO: implement!

    // Remember to come back to this !!!!!!!!!!!
    (void)snake_p; 

    if (!cells_p || !width_p || !height_p || !compressed) {
        return INIT_ERR_BAD_CHAR;
    }

    char* p = compressed;
    if (*p != 'B') return INIT_ERR_BAD_CHAR;
    p++;

    // parse height (first number after `B`)
    if (!isdigit((unsigned char)*p)) return INIT_ERR_BAD_CHAR;
    size_t h = 0;
    while (isdigit((unsigned char)*p)) {
        h = h * 10 + (size_t)(*p - '0');
        p++;
    }

    if (*p != 'x') return INIT_ERR_BAD_CHAR;
    p++;

    // parse width (number after `x`)
    if (!isdigit((unsigned char)*p)) return INIT_ERR_BAD_CHAR;
    size_t w = 0;
    while (isdigit((unsigned char)*p)) {
        w = w * 10 + (size_t)(*p - '0');
        p++;
    }

    if (*p != '|') return INIT_ERR_BAD_CHAR;
    p++;

    // allocate spaces for w * h cells
    int* cells = malloc(sizeof(int) * w * h);
    if (!cells) return INIT_ERR_BAD_CHAR;

    // fill by reading runs until we've produced exactly w * h cells
    size_t idx = 0;
    size_t row = 0;
    size_t col =0;
    size_t snake_count = 0;

    while (*p != '\0') {
        if (*p == '|') {
            if (col != w) {
                free(cells);
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }
            row++;
            if (row >= h) {
                free(cells);
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }
            col = 0;
            p++;
            continue;
        }

    // letter
    char kind = *p;
    int cell_val = cell_from_char(kind);
    if (cell_val < 0) { free(cells); return INIT_ERR_BAD_CHAR; }
    p++;

    // number after the letter
    if (!isdigit((unsigned char)*p)) { free(cells); return INIT_ERR_BAD_CHAR; }
    size_t count = 0;
    while (isdigit((unsigned char)*p)) {
      count = count * 10 + (size_t)(*p - '0');
      p++;
    }

     // emit count cells
    for (size_t k = 0; k < count; k++) {
        if (col >= w || idx >= w * h) { free(cells); return INIT_ERR_INCORRECT_DIMENSIONS; }
        cells[idx++] = cell_val;
        if (cell_val == FLAG_SNAKE) {
            snake_count ++;
        }
        col++;
        }
    }

    if (col != w) {
        free(cells);
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }
    row++;

    if (row != h || idx != w * h) {
        free(cells);
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }

    if (snake_count != 1) {
        free(cells);
        return INIT_ERR_WRONG_SNAKE_NUM;
    }
    
    // success
    *cells_p = cells;
    *width_p = w;
    *height_p = h;
    return INIT_SUCCESS;
}