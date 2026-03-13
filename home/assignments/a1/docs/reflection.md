# Assignment 1 Reflection
**Name:** Joshua Benros

## Overview

This document is a reflection on my experience completing Assignment 1. The goal was to build a Unix-style text-processing pipeline twice — once in C and once in Bash — and make sure both versions produced identical output. I also completed the extra credit portion, which involved writing a recursive file system traversal program (fsrecursive).

Overall, this assignment ended up being much deeper than I initially expected. It wasn’t just about writing programs that worked — it was about understanding how data flows through pipelines, how memory behaves in C, and how modular command-line tools are designed to interact.

--- 

## What Changed Between the Design Document and the Final Implementation

When I wrote my design document, I had a general understanding of what needed to be built, but the implementation required much more detail and precision than I anticipated.

In the design phase, I identified `sort` as the most difficult constraint, and that prediction was accurate. I originally imagined implementing a straightforward sorting function, but I quickly realized that handling an unknown number of input lines meant dynamically allocating memory. That led to implementing a resizing strategy using `realloc`, carefully tracking capacity, and ensuring memory was freed correctly.

For `mismatch`, I initially thought a basic dictionary scan might be sufficient. However, during implementation, it became clear that repeatedly scanning a dictionary file for each word would be inefficient. I ended up implementing a hash table with chaining to support fast lookups. This added complexity, collision handling, hashing logic, and cleanup.

The design document also simplified input handling. In practice, I had to carefully manage fixed-size buffers, handle edge cases like missing newlines at EOF, strip trailing newline characters, and ensure consecutive whitespace did not produce empty output lines. This part of the assignment forced me to slow down and think more carefully about how text parsing works at the systems level.

The extra credit `fsrecursive` component introduced an entirely new level of complexity. I had to research and understand how `scandir`, `lstat`, `struct dirent`, and `struct stat` work. I also had to think about how to safely construct child paths and avoid infinite recursion through symbolic links. This part was especially challenging for me.

--- 

## What Worked Well

One thing that worked very well was sticking to modular design principles: each program reads from standard input, writes to standard output, and performs one specific transformation. This approach made debugging much easier. If something broke, I could isolate the stage that was failing instead of guessing where the issue was in the entire pipeline.

Starting with the C implementations before writing the Bash scripts also helped. It forced me to truly understand what each stage was doing instead of relying on built-in utilities. Once the C pipeline worked correctly, recreating it in Bash felt structured and intentional.

Testing incrementally was another strength. Rather than building everything and hoping it worked, I tested each program independently, then gradually combined them into a full pipeline.

---

## What I Struggled with

At the beginning, I struggled with fully understanding how pipes move data between processes. I had to think through when data is flushed, how buffering works, and why `stderr` behaves differently from `stdout`. There were moments where output did not appear as expected, and I had to step back and reason about what each process was actually receiving.

Memory management in C was another major challenge. I had to be careful not to leak memory, not to double free, and to handle resizing arrays safely. Using `realloc` correctly and managing dynamically allocated strings required more discipline than I initially expected.

For recursion, I had to research topics that were not deeply covered in the design document. Understanding the difference between `stat` and `lstat`, how symbolic links behave, and how directory traversal works at the system level required additional reading and testing.

---

## Time Spent 

This assignment took about 20 hours to complete. A large portion of that time was spent understanding concepts rather than just writing code. I had to slow down, test small pieces, and think carefully about how data and memory were being handled at each stage.

---

## UNIT Test Proposal

A strong unit test for this assignment is an end-to-end pipeline consistency test. The purpose of this test is to verify that both the C and Bash pipelines produce identical output for the same inputs.

For example, if `unix_sentence.text` contains: Hello hello WORLD test Test and `unix_dict.text` contains: hello test, the expected final output should be:

```
world
```

This test confirms that words are split correctly, converted to lowercase, sorted properly, deduplicated, and filtered against the dictionary. If both implementations produce exactly the same output, it confirms that each stage in the transformation chain is working correctly and that the overall pipeline behaves as intended.

--- 

## Final Thoughts

This assignment changed how I think about building small tools. It reinforced the importance of modular design, careful memory management, and understanding how data flows between processes. The recursive file system component especially pushed me outside my comfort zone and required real research and debugging.

I feel like I gained a deeper understanding of how Unix-style tools are constructed and how systems-level programming works under the hood.