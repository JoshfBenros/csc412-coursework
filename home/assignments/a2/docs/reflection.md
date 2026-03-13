# CSC412 Assignment 2 – Reflection
**Name:** Joshua Benros

## Overview

This document reflects on my experience completing Assignment 2, where the goal was to implement the Snake game in C using the provided stencil code. The project required building core mechanics such as board initialization, movement logic, collision detection, board decompression, snake growth with a linked list, and UTF-8 name handling.

The assignment was structured in phases, which helped guide my development process. Early phases focused on getting the game running with basic mechanics, while later phases introduced more complex features like board decompression and linked-list-based growth.

Overall, this assignment pushed me to think more carefully about program structure, memory management, and how multiple systems interact within a larger program.

---

## What Changed Between the Design Document and the Final Implementation

When writing my design document, I outlined a modular approach for implementing the game. My plan was to build it in stages: board initialization, movement logic, decompression, and linked list growth. That overall structure stayed mostly the same during implementation. However, once I started writing code and running the provided tests, I realized that some parts needed more detailed logic than I initially expected. 

Even though some functions required small changes, the modular structure from the design document helped a lot. Since the systems were separated clearly, I was able to adjust specific parts of the implementation without breaking other parts of the program.

---

## What Worked Well

One thing that worked very well during this assignment was breaking the project into modular components. Instead of trying to implement everything at once, I focused on one subsystem at a time. For example, I worked on board initialization and memory setup first, then movement and collision logic, and later added more complex features such as decompression and snake growth.

Writing the design document before implementing the project also helped me stay organized. Because I had already documented the overall structure of the program, it was easier to track where different pieces of logic belonged. When I needed to revisit parts of the code later, the design plan helped me understand how everything fit together.

Another thing that worked well was testing frequently. Running the provided tests after implementing each feature helped me catch mistakes early and verify that each subsystem was working correctly before moving on.

---

## What I Struggled With

The most challenging part of this assignment for me was implementing the board decompression logic. Parsing the compressed board format in C required precise string processing and validation. I had to verify that the board dimensions matched the expected size and that the correct number of snake segments existed.

Working with the linked list that represents the snake also took time to fully understand. Each update step inserts a new head position while removing the tail unless the snake is growing. Keeping the linked list structure and board representation synchronized required consistent update logic.

Debugging also took time because small mistakes in board indexing or update logic could cause tests to fail in ways that were not immediately obvious. In those cases, I had to slow down and trace through what the program was doing step by step.

---

## Time Spent

This assignment took me about 20 hours to complete.

A good portion of that time was spent understanding how the different systems in the stencil code worked and interacted. Implementing the core game mechanics took time, but debugging and ensuring that all provided tests passed also required significant effort.

---

## Unit Test Proposal

One useful unit test for this assignment is verifying that the snake grows correctly after consuming food. In this test, the board would be initialized with the snake positioned directly next to a food tile. The input trace would simulate the snake moving into the food cell during an update step. The expected behavior is that the snake grows by one segment, the score increases, and the tail is not removed during that cycle. In addition, a new food item should be placed on the board. This test confirms that several core systems are working together correctly: movement logic, food detection, score updates, and linked list growth. If any of these systems are implemented incorrectly, the snake would fail to grow or the board state would not match the expected output.

---

## Final Thoughts

Overall, this assignment reinforced the importance of modular program design and memory management in C. Building the game step by step made the project easier to reason about and debug. The linked list implementation and decompression logic were definitely the most challenging parts, but working through them gave me a better understanding of how data structures and memory interact in a larger program.

By the end of the assignment, I felt much more comfortable navigating and modifying a larger codebase while keeping different systems organized and working together.