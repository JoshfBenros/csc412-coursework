# Assignment 2 Design Document 
**Name:** Joshua Benros
**Approach** Planning First 

## Overview 

This assignment involves implementing the Snake game in C using the provided stencil code. The game is built around a dynamically allocated 2D board stored as a 1D array of integers (`int* cells`). Each cell uses bit flags to represent walls, snake segments, food, or empty space. 

The game runs in a loop that: 

- Sleeps briefly (`usleep`)
- Processes user input
- Updates the game state (`update`)
- Renders the board (`render_game`)
- Ends when a game-over condition is reached

The project gradually increases in complexity: starting with basic movement and collision detection, then adding custom board decompression, snake growth using a linked list, UTF-8 string handling, and finally custom test creation.

---

### Problem Description 

The goal is to implement the core mechanics of Snake while respecting the architecture defined in the stencil code. The game state must be stored in memory safely, updated correctly every tick, and rendered accurately.

Key components include:
- Dynamically allocated board (`int* cells`)
- Global game state (`g_game_over`, `g_score`)
- Snake representation (`snake_t` with linked list body)
- Input handling (`get_input`)
- Board decompression (`decompress_board_str`)
- Food placement (`place_food`)
- Game state updates (`update`)
- Cleanup (`teardown`)
- Rendering (`render_game`, `render_game_over`)
- UTF-8 name handling (`read_name`, `mbslen`)

The final implementation must pass all provided tests along with additional tests that I've written. 

---

## How I Plan to Solve the Problem 

I will approach this assignment incrementally, implementing and testing one subsystem at a time. 

### Ensure stencil compiles cleanly

- Confirm all required globals are defined.
- Verify function prototypes match implementations.
- Ensure memory allocation and cleanup paths are correct. 

### Implement Base Board Initialization

Using:
- `initialize_game`
- `initialize_default_board`

Steps:
- Allocate board memory correctly.
- Set width and height.
- Initialize snake state. 
- Verify no memory leaks using sanitizer.
- Ensure `teardown` properly frees allocated memory.

### Implement game loop struccture (in `snake.c`)

The loop will:
- Call `usleep`
- Call `get_input`
- Call `update`
- Call `render_game`
- Continue until `g_game_over` becomes true 

### Implement movement logic (`update`)

- Compute next head position.
- Detect wall collisions.
- Detect snake self-collision.
- Update board flags correctly. 
- Move snake body accordingly.

### Add Food Mechanics

Using: `place_food`

Steps:
- Detect food collisino. 
- Increment `g_score`.
- Grow snake if needed. 
- Place new food.

### Add input handling

Using:
- `get-input`
-  `enum input_key`

Steps:
- Validate direction changes.
- Prevent illegal reverse movement if required.

### Implement board decompression

Using:
- `decompress_board_str`
- `enum board_init_status`

Steps:
- Parse rows and tokens safely.
- Validate board dimensions.
- Handle errors correctly.

### Convert snake to linked list (Growth Support)

Using:
- `linked_list` helpers 
    - `insert_first`
    - `remove_last`
    - `get_first`
    - `get_last`

Steps:
- Insert new head each tick.
- Remove tail unless growing.
- Keep linked list synchronized with board memory.

### Add UTF-8 name support

Using:
- `read_name`
- `mbslen`
- `render_game_over`

Steps:
- Read name from stdin.
- Compute length using UTF-8 logic.
- Render game-over screen with correct name length.

### Cleanup and Game End

When game ends:
- `end_game` handles final rendering.
- `teardown` frees allocated memory and linked list data.

### Write custom test

- Add new traces in test directory.
- Cover edge cases.
- Validate decompression errors.
- Validate growth behavior.

---

### Implementation Order

- Fix compilation issues and globals.
- Board initialization + memory cleanup.
- Game loop structure.
- Basic snake movement.
- Collision detection.
- Food logic (`place_food`).
- Input handling.
- RLE decompression (`decompress_board_str`).
- Linked list snake growth.
- UTF-8 name handling (`mbslen`).
- Custom tests.

---

### Reason 

This order prioritizes:
- Building a minimal working version first.
- Stabilizing memory management early.
- Verifying board indexing logic before parsing compressed input.
- Leaving linked list growth and UTF-8 handling for after core mechanics are stable.

By structuring the implementation this way, debugging becomes easier because each subsystem is tested independently before adding complexity.

---

### Difficult or Time-Consuming Areas

RLE Board Decompression
- Safe string parsing in C.
- Dimension validation.
- Correct error return handling.

Snake Growth Using Linked List
- Maintaining correct head/tail updates.
- Handling case where snake moves into previous tail position.
- Keeping linked list and board representation synchronized.

UTF-8 Length Calculation
- Counting code points instead of bytes. 
- Correctly identifying leading byte patterns.

Memory Management
- Avoiding leaks.
- Avoiding double free.
- Ensuring `teardown` always runs correctly.

---

## Research

- Dynamic memory allocation patterns in C.
- Safe string parsing techniques (`strtok`, `strtol`).
- Bitwise operations for flag checking.
- Doubly linked list manipulation.
- UTF-8 encoding byte structure.
- Debugging with `gdb` and sanitizer tools.

---

## Pseudocode ........

### `update` (cells,width, height, `snake_p`, input, growing)

// Updates the game state for one tick of the game loop.
// Handles movement, collision detection, food logic,
// and snake growth using the linked list structure.

update(...):

    // If game already ended, do nothing
    if g_game_over == 1:
        return

    // Determine the snake's direction based on user input
    update snake direction if input is valid

    // Compute the next head position based on direction
    next_position = current_head + direction_delta

    // Check for wall collision
    if next_position contains WALL:
        set g_game_over = 1
        return

    // Check for self-collision
    if next_position contains SNAKE BODY:
        set g_game_over = 1
        return

    // Check for food collision
    if next_position contains FOOD:
        increment g_score
        set growing = 1
        place_food(...)

    // Insert new head into linked list
    insert_first(snake body list, next_position)
    mark board cell as SNAKE

    // If not growing, remove tail
    if growing == 0:
        remove_last(snake body list)
        clear old tail cell on board

### `decomplress_board_str` (`cells_p`, `width_p`, `height_p`, `snake_p`, compressed)

// Parses a run-length encoded board string.
// Allocates board memory, fills cells, validates dimensions, and ensures correct snake count.

decompress_board_str(...):

    // Parse width and height from compressed string
    extract dimensions

    // Allocate board memory
    allocate width * height cells

    snake_count = 0
    current_index = 0

    // Process each (character, count) pair
    for each token in compressed:
        validate character type

        repeat count times:
            if current_index exceeds board size:
                return INIT_ERR_INCORRECT_DIMENSIONS

            fill cell at current_index
            if cell is SNAKE:
                increment snake_count

            current_index += 1

    // Validate final board state
    if current_index != width * height:
        return INIT_ERR_INCORRECT_DIMENSIONS

    if snake_count is incorrect:
        return INIT_ERR_WRONG_SNAKE_NUM

    return INIT_SUCCESS