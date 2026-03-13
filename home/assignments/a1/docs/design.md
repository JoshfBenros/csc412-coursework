# Assignment 1 Design Document 
**Name:** Joshua Benros
**Approach:** Planning First 

## Overview

This document describes my understanding of the problem, my planned approach to solving it, anticipated challenges, and areas requiring additional research before implementation.

---

### Problem Description 

The problem is to take in inputs from two data files (`unix_sentence.text` and `unix_dict.text`), process that data through a sequence of piped programs, and ensure that two separate implementations of this pipeline (one in C and one in Bash) produce an identical final output.

---

## How I Plan to Solve the Problem 

We solve this problem by implementing six small, single-purpose command-line tools twice: once as compiled C programs and once as Bash scripts. Each tool reads standard input and writes to standard output. We then use Bash pipelines to connect the tools in the required order and redirect the final output into `unix_c_output.text` and `unix_bash_output.text`. Finally, we verify correctness by checking that these two output files match exactly.

---

## Implementation Order 

I'm choosing to start with C first because I'm still building confidence with stdin/stdout and pipelines. Writing the tools in C forces me to be precise about input/output behavior and edge cases, and it lets me test each stage in isolation before connecting everything.

I will start with `sentence`, then `makewords`, then `lowercase`, followed by `unique`, and `mismatch`. I will implement `sort` last because it has the strictest constraints (no Linux `sort` and no `qsort`) and will likely be the most time consuming. After the C pipeline works end to end, I will implement the Bash scripts in the same order and verify that the Bash pipeline output matches the C pipeline output exactly. 

---

### Reason 

`sentence` - It's the entry point 
`makeword` - establishes the "one word per line" format everything after relies on 
`lowercase` - simple transformation
`unique` - simpler than sort
`mismatch` - introduces the second file
`sort` - the hardest constraint, plus I want to make it optomized 

---

## Difficult or Time-Consuming Areas 

One potentially difficult area is implementing the `sort` functionality, since I cannot use Linux `sort` or C `qsort`, and I want the result to be reasonably efficient for performance benchmarks. Another challenging area is the `mismatch` tool, which requires efficiently comparing streamed input words against a dictionary file while handling edge cases consistently.

---

## Research

Each tool will be implemented as a standalone program that reads from standard input and writes to standard output. The C implementations may use helper functions and reusable library code for file I/O and string processing, while avoiding duplicated logic. Data structures such as dynamic arrays or hash-based structures may be used to store words for sorting, deduplication, and dictionary comparison.

---

#### Pseudocode: makewords (Handling Consecutive Whitespace)

One potential edge case for `makewords` is handling multiple consecutive whitespace characters (e.g., two or more spaces in a row). An approach that only checks for exactly two spaces would fail if there are more than two or if other whitespace characters are involved.

To handle this the program should track whether it is currently inside a word. Characters are appended to the current word until a whitespace character is encountered. When whitespace is detected, the program only outputs the current word if it is non-empty, then resets for the next word.

Pseudocode:

- initialize an empty buffer for the current word
- while reading characters from stdin:
  - if the character is not whitespace:
    - append character to current word buffer
  - else (character is whitespace):
    - if current word buffer is not empty:
      - output the word followed by a newline
      - clear the word buffer
- after input ends:
  - if current word buffer is not empty:
    - output the final word

This approach ensures that any number of consecutive spaces, tabs, or newlines do not produce empty output lines.

---

#### Pseudocode: mismatch (Dictionary Comparison)

The `mismatch` tool reads words from standard input and compares them against a dictionary file. To avoid repeatedly scanning the dictionary file for each input word, the dictionary should be loaded into a data structure that supports fast lookup.

Pseudocode:

- open dictionary file
- read each word from dictionary file:
  - insert word into a lookup structure (e.g., hash set or sorted array)
- while reading words from stdin:
  - if word is not found in dictionary structure:
    - output word to stdout
