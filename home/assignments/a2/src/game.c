#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linked_list.h"
#include "mbstrings.h"

static int ensure_snake_initialized(int* cells, size_t width, size_t height,
                                    snake_t* snake_p) {
    if (!snake_p) {
        return 0;
    }
    if (snake_p->initialized) {
        return 1;
    }

    size_t total = width * height;
    size_t head = total;
    for (size_t i = 0; i < total; i++) {
        if (cells[i] & FLAG_SNAKE) {
            head = i;
            break;
        }
    }
    if (head == total) {
        return 0;
    }

    snake_p->positions = malloc(sizeof(size_t));
    if (!snake_p->positions) {
        return 0;
    }
    snake_p->positions[0] = head;
    snake_p->length = 1;
    snake_p->capacity = 1;
    if (snake_p->dx == 0 && snake_p->dy == 0) {
        snake_p->dx = 1;
        snake_p->dy = 0;
    }
    snake_p->initialized = 1;
    return 1;
}

static int ensure_capacity(snake_t* snake_p, size_t needed) {
    if (snake_p->capacity >= needed) {
        return 1;
    }

    size_t new_cap = snake_p->capacity == 0 ? 4 : snake_p->capacity * 2;
    while (new_cap < needed) {
        new_cap *= 2;
    }

    size_t* grown = realloc(snake_p->positions, sizeof(size_t) * new_cap);
    if (!grown) {
        return 0;
    }
    snake_p->positions = grown;
    snake_p->capacity = new_cap;
    return 1;
}

/** Updates the game by a single step, and modifies the game information
 * accordingly. Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: width of the board.
 *  - height: height of the board.
 *  - snake_p: pointer to your snake struct (not used until part 2!)
 *  - input: the next input.
 *  - growing: 0 if the snake does not grow on eating, 1 if it does.
 */

void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    // `update` should update the board, your snake's data, and global
    // variables representing game information to reflect new state. If in the
    // updated position, the snake runs into a wall or itself, it will not move
    // and global variable g_game_over will be 1. Otherwise, it will be moved
    // to the new position. If the snake eats food, the game score (`g_score`)
    // increases by 1. This function assumes that the board is surrounded by
    // walls, so it does not handle the case where a snake runs off the board.

    if (g_game_over) {
        return;
    }
    if (!cells || !snake_p) {
        return;
    }
    if (!ensure_snake_initialized(cells, width, height, snake_p)) {
        return;
    }

    size_t head = snake_p->positions[0];
    size_t tail = snake_p->positions[snake_p->length - 1];

    int dx = snake_p->dx;
    int dy = snake_p->dy;
    switch (input) {
        case INPUT_UP:
            dx = 0;
            dy = -1;
            break;
        case INPUT_DOWN:
            dx = 0;
            dy = 1;
            break;
        case INPUT_LEFT:
            dx = -1;
            dy = 0;
            break;
        case INPUT_RIGHT:
            dx = 1;
            dy = 0;
            break;
        case INPUT_NONE:
        default:
            break;
    }

    // For a snake of length > 1, disallow immediate reversal.
    if (snake_p->length > 1 && dx == -snake_p->dx && dy == -snake_p->dy) {
        dx = snake_p->dx;
        dy = snake_p->dy;
    }
    snake_p->dx = dx;
    snake_p->dy = dy;

    int hx = (int)(head % width);
    int hy = (int)(head / width);
    int nx = hx + dx;
    int ny = hy + dy;
    size_t next = (size_t)ny * width + (size_t)nx;

    if (cells[next] & FLAG_WALL) {
        g_game_over = 1;
        return;
    }

    int ate_food = (cells[next] & FLAG_FOOD) != 0;
    int should_grow = ate_food && growing;

    // Moving into the current tail is legal if the tail will move this frame.
    if ((cells[next] & FLAG_SNAKE) && !(next == tail && !should_grow)) {
        g_game_over = 1;
        return;
    }

    if (!ensure_capacity(snake_p, snake_p->length + 1)) {
        g_game_over = 1;
        return;
    }

    memmove(&snake_p->positions[1], &snake_p->positions[0],
            sizeof(size_t) * snake_p->length);
    snake_p->positions[0] = next;
    snake_p->length++;

    cells[next] = FLAG_SNAKE;

    if (!should_grow) {
        size_t old_tail = snake_p->positions[snake_p->length - 1];
        if (old_tail != next){
            cells[old_tail] = FLAG_PLAIN_CELL;
        }
        snake_p->length--;
    }

    if (ate_food) {
        g_score += 1;
        place_food(cells, width, height);
    }
}

/** Sets a random space on the given board to food.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: the width of the board
 *  - height: the height of the board
 */
void place_food(int* cells, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    if (*(cells + food_index) == FLAG_PLAIN_CELL) {
        *(cells + food_index) = FLAG_FOOD;
    } else {
        place_food(cells, width, height);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Prompts the user for their name and saves it in the given buffer.
 * Arguments:
 *  - `write_into`: a pointer to the buffer to be written into.
 */
void read_name(char* write_into) {
    if (!write_into) {
        return;
    }

    if (!fgets(write_into, 1000, stdin)) {
        write_into[0] = '\0';
        return;
    }

    size_t len = strlen(write_into);
    if (len > 0 && write_into[len - 1] == '\n') {
        write_into[len - 1] = '\0';
    }
}

/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - snake_p: a pointer to your snake struct. (not needed until part 2)
 */
void teardown(int* cells, snake_t* snake_p) {
    if (cells) {
        free(cells);
    }
    if (snake_p && snake_p->positions) {
        free(snake_p->positions);
        snake_p->positions = NULL;
        snake_p->length = 0;
        snake_p->capacity = 0;
        snake_p->initialized = 0;
    }
}
