# Assignment 3 Design Document
**Name:** Joshua Benros  
**Approach:** Planning First

## Overview

Assignment 3 focuses on implementing DMalloc, a debugging memory allocator written in C. Instead of simply allocating and freeing memory like the standard `malloc` and `free`, this allocator adds additional checks and tracking features to help detect memory-related bugs in programs.

The allocator must track statistics about memory usage, detect invalid frees and double frees, protect against integer overflow during allocation, detect boundary write errors, and report memory leaks that remain at the end of program execution.

Overall, the goal of this assignment is to better understand how memory allocators work internally and how debugging tools can detect common memory errors.

## Problem Description

The main goal of this assignment is to implement a debugging version of `malloc` and `free`. The allocator must keep track of information about every allocation and use that information to detect incorrect behavior.

The allocator must support the following behaviors:

- Track statistics about allocations and frees
- Detect invalid frees (freeing memory that was never allocated)
- Detect double frees (freeing the same pointer twice)
- Protect against integer overflow when calculating allocation sizes
- Detect boundary write errors where a program writes outside the allocated block
- Report memory leaks that remain at the end of execution

## How I Plan to Solve the Problem

To implement the allocator, I will break the problem into several smaller pieces and implement them incrementally.

### 1. Track Allocation Metadata

Each allocation will store metadata that describes the allocated block. This metadata will likely include:

- Size of the allocation
- Whether the block is currently active
- Information needed for statistics
- Possibly file and line information depending on the interface
- Boundary protection values

The metadata will be stored in memory immediately before the user-visible memory block so that it can be accessed later when `dmalloc_free` is called.

### 2. Implement Allocation Statistics

The allocator must maintain global statistics that track memory usage across the program. These statistics will include values such as:

- Total number of allocations
- Number of currently active allocations
- Total number of bytes allocated
- Number of failed allocations
- Number of currently active bytes

These values will be updated every time `dmalloc_malloc` or `dmalloc_free` is called.

### 3. Implement `dmalloc_malloc`

The `dmalloc_malloc` function will handle allocating memory and preparing it for safe usage.

The steps will include:

- Validate the requested allocation size
- Check for integer overflow when calculating the total size required for metadata and boundary protection
- Request the correct amount of raw memory
- Initialize the metadata for the block
- Place any boundary protection values around the user allocation
- Update allocation statistics
- Return a pointer to the user-visible portion of the allocation

The pointer returned to the user must point only to the memory they requested, not the metadata.

### 4. Implement `dmalloc_free`

The `dmalloc_free` function will safely free memory and detect incorrect usage.

The steps will include:

- Handle the case where the pointer is `NULL`
- Determine whether the pointer belongs to a valid allocation
- Recover the metadata associated with the allocation
- Check if the block has already been freed
- Verify that the boundary protection values are still intact
- Update allocation statistics
- Mark the block as freed and release the memory

These checks will help detect both invalid frees and double frees.

### 5. Add Invalid Free and Double Free Detection

To detect invalid frees, the allocator must verify that the pointer corresponds to a known allocation.

To detect double frees, the metadata for each block will track whether the block has already been freed. If a free operation occurs on an already freed block, the allocator will report the error.

### 6. Add Integer Overflow Protection

When calculating the total memory required for an allocation, the allocator must ensure that adding metadata and boundary protection does not cause an integer overflow.

If an overflow would occur, the allocator should treat the allocation as a failure and update the failure statistics.

### 7. Add Boundary Write Detection

To detect writes outside the allocated region, the allocator will place a sentinel value (canary) near the boundary of the allocated block.

When the block is freed, the allocator will check whether this value has been modified. If the value has changed, this indicates that a boundary write occurred.

### 8. Add Memory Leak Reporting

At the end of program execution, the allocator must report any memory blocks that were allocated but never freed.

This requires keeping track of active allocations so they can be reported when the program finishes.

### 9. Test Important Edge Cases

Once the core features are implemented, I will test the allocator using edge cases such as:

- Freeing `NULL`
- Freeing the same pointer twice
- Freeing pointers that were never allocated
- Extremely large allocation sizes
- Writing beyond the allocated memory
- Leaving memory unfreed to confirm leak detection works

## Implementation Order

I plan to implement the allocator in the following order:

1. Understand the provided interfaces and required statistics
2. Design the metadata structure for allocations
3. Implement basic allocation statistics tracking
4. Implement a basic version of `dmalloc_malloc`
5. Implement a basic version of `dmalloc_free`
6. Add detection for invalid frees and double frees
7. Add integer overflow protection
8. Add boundary write detection
9. Implement memory leak reporting
10. Test the allocator with edge cases

## Reason

This order allows me to implement the allocator gradually while keeping the system stable.

The first steps focus on building the core allocation and tracking system, which everything else depends on. Once basic allocation and freeing work correctly, I can add additional safety checks such as invalid free detection, overflow protection, and boundary checking.

By separating the debugging features from the core functionality, it will be easier to isolate bugs and verify that each component works correctly before moving on to the next feature.

## Difficult or Time-Consuming Areas

### Metadata, Invalid Free Checks, and Boundary Writes

One time-consuming part of this assignment will be getting metadata layout and pointer arithmetic correct, since small offset mistakes can break allocation and free behavior. Another difficult area is validating frees safely, because the allocator must reject invalid or already-freed pointers without crashing. Boundary write detection is also tricky, since canary placement and checks must be consistent to avoid missed corruption or false errors. These checks will likely require careful step-by-step testing and debugging.

## Research

To prepare for this assignment, I may need to review the following topics:

- Pointer arithmetic in C
- Struct layout and memory offsets
- Safe integer arithmetic and overflow detection
- Debugging allocators and memory safety tools
- Strategies for detecting memory corruption

Understanding these topics will help ensure the allocator is implemented safely and correctly.

## Pseudocode

Below is a simplified outline of how allocation and free operations might work.

### Allocation

function dmalloc_malloc(size):

    if size is invalid:
        update failure statistics
        return NULL

    if size + metadata + boundary exceeds limit:
        update failure statistics
        return NULL

    allocate raw memory

    initialize metadata

    place boundary protection values

    update statistics

    return pointer to user memory

### Free

function dmalloc_free(ptr):

    if ptr is NULL:
        return

    locate metadata for ptr

    if metadata is invalid:
        report invalid free

    if block already freed:
        report double free

    check boundary protection values

    update statistics

    mark block as freed
