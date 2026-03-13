# Module 1 Design Doc 
**Name:** Joshua Benros
**Approach:** Planning First Approach 

--- 

## Overview
This assignment focuses on working with characters, strings, and arrays in C while reinforcing pointer usage, memory safety, and Unix-based workflows. The primary goal is to implement multiple string and array reversal functions using different approaches while passing a provided unit test suite.

--- 

## Planned Order of Implementation
- Review the assignment handout in its entirety to ensure all requirements and constraints are understood.
- Review the provided stencil code in `reverse.c`, `reverse.h`, and `revtest.c` to understand the expected function signatures and testing structure.
- Implement helper functions first (`swap_chr` and `swap`) to support reuse and reduce duplicated logic.
- Implement `reverse_str1` using a safe in-place reversal strategy.
- Implement `reverse_arr1` to reverse an array of strings by swapping pointers.
- Gradually uncomment and run unit tests in `revtest.c` to validate correctness after each implementation.
- Implement additional reverse functions with unique logic to pass more unit tests.
- Complete Part 2 by updating argument handling in `revtest.c` and implementing the required logic in `build.sh`.

---

## Two areas that will be difficult or time-consuming 
- Ensuring all files (`.h`, `.c`, and `.sh`) work together correctly during compilation and execution, especially understanding how function declarations and implementations are linked.
- Ensuring that reverse functions are meaningfully unique in their logic while still being correct and memory-safe, without copying or slightly modifying existing implementations.

---

## 3 Questions and Topics to Research 

Before beginning implementation, I identified the following questions and knowledge gaps to resolve to safely and correctly complete this assignment. 

---

### 3.1 Strings and Character Pointers in C

**Q: Is every string a 'char*'?**
In C, strings are technically arrays of characters (`char[]`) that end with a null terminator (`'\0'`). When passed to functions, these arrays decay into pointers (`char*`). Conceptually, strings are accessed via `char*`, but they are stored as contiguous character arrays in memory

**Q: Why is the null terminator important?**
The null terminator marks the end of a string. Functions like `printf` and loops that process strings rely on `'\0'` to know when to stop. Failing to account for it can cause out-of-bounds memory access and segmentation faults. 

**Q: When reversing a string, should I rely on length or `'0'`?**
The length is explicitly provided, so reversing should be done using the given length. This avoids scanning memory unnecessarily and ensures the null terminator is not accidentally moved or overwritten.

---

### 3.2 Pointer Dereferencing and Memory Access

**Q: Do you always dereference a string pointer in loops?**
No. Dereferencing is only needed when accessing or modifying the character stored at an address. Pointer arithmetic (moving the pointer or index) does not require dereferencing. For example, incrementing a pointer moves through memory, while dereferencing retrieves the character at that location. 

**Q: What causes segmentation faults?**
Common causes include accessing memory out of bounds, dereferencing invalid or uninitialized pointers, incorrectly handling the null terminator, or swapping the wrong pointer types 

---

### 3.3 `char*` vs `char**`

**Q: What does `char**` represent?**
A `char**` represents a pointer to a pointer to a character. It is used to represent an array of strings, where each element is a `char*` pointing to the first character of a string. 

**Q: When reversing an array of strings, what is being swapped?**
The pointers to strings (`char*`) are swapped, not the individual characters inside the strings. This is more efficient and avoids unnecessary character-level copying.

**Q: Why is swapping `char*` different from swapping `char`?**
Swapping `char*` values swap addresses (entire strings), while swapping `char` values swap individual characters. Confusing these can lead to incorrect behavior or memory errors. 

---

#### Section Summary 

These questions were researched and resolved prior to implementation to reduce debugging time, avoid segmentation faults, and ensure correct pointer and memory usage.

---

## Outline

### Helper Functions
- `swap_chr(char* a, char* b)`
  - Swap two characters in memory using a temporary variable.
  - This helper function will be reused by string-reversal functions.

- `swap(char** a, char** b)`
  - Swap two string pointers in an array.
  - Used when reversing arrays of strings to avoid character-level copying.

### String Reversal
- `reverse_str1(char* str, int len)`
  - Reverse a string in place using a two-index or two-pointer approach.
  - Repeatedly swap characters from the beginning and end of the string while moving inward.
  - Ensure the null terminator is not modified.

### Array Reversal
- `reverse_arr1(char** arr, int num)`
  - Reverse an array of strings by swapping `char*` elements.
  - Use indices or pointers to traverse from both ends of the array.

### Testing
- Use the provided unit tests in `revtest.c`.
- Incrementally enable tests to isolate failures.
- Recompile and test after each major change to avoid regressions.