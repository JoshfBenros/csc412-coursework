# Module 1 Reflection

## Overview
This reflection revisits the original design decisions outlined in design.md and explains how those plans evolved once real test cases, edge conditions, and debugging challenges were introduced.

This module was a strong reminder that fundamentals matter more than anything else, especially in C. On the surface, reversing arrays and strings seems simple, but the deeper I got into the assignment, the more I realized how much small details around memory, pointers, and edge cases actually affect correctness.

A big part of this assignment wasn’t just writing code that “works once,” but writing code that behaves correctly under many different conditions. That mindset shift ended up being one of the most valuable takeaways.

--- 

## Design vs. Final Implementation
When I started, my design was very straightforward: use two indices or pointers, swap elements, and move inward until the string or array is reversed. That approach worked fine for the basic cases.

As I progressed, the final implementation became more structured and defensive. I introduced helper functions to normalize sizes, detect leading and trailing spaces, and make decisions based on how the input was structured. 

Overall, the final version was more intentional and easier to reason about than my original plan.

--- 

## What Worked / What Didn’t
What worked well was breaking the problem into smaller pieces. Once I stopped trying to fix everything inside one function and instead created helper logic, debugging became much easier. Writing and reusing small helpers also made my code more consistent across different reversal functions.

What didn’t work at first was trying to handle all edge cases at once. I kept running into failures, making changes, and then getting confused about which change caused which behavior. Once I slowed down and approached each test case individually, the logic became much clearer.

--- 

## Issues and Research
The biggest challenge was dealing with edge cases, especially strings with leading spaces, trailing spaces, or both. These cases forced me to think differently about what “reversing a string” actually means when whitespace and null terminators are involved.

I also had to revisit some fundamentals that I thought I already understood, such as:

- The difference between swapping pointers vs. swapping characters

- How null terminators affect string length and indexing

- Why treating test cases as an external system (rather than “wrong”) is important

Another helpful debugging technique I used was adding temporary print statements. Seeing index values, characters being swapped, and intermediate string states helped me understand exactly where my logic was going wrong instead of guessing.

--- 

## Time Spent
I spent significantly more time on this assignment than I expected. A large portion of that time was spent debugging and rethinking my approach rather than writing new code. In hindsight, if I had a stronger grasp of pointer behavior and edge-case handling upfront, I could have completed it much faster. Overall, I would estimate around 10 hours.

--- 

## Unit Test (Example)
**Purpose:** Verify that string reversal works correctly when the input contains trailing spaces, which require special handling.

**Input:** The string `"trailing space  "` with size 16.

**Expected Output:** `"  ecapS gniliart"`

**Why it works:** 
This test confirms two things at once:

- The string is reversed correctly in place.
- The edge case involving trailing spaces is handled properly, ensuring the space is preserved and the reversal logic does not corrupt memory or mis-handle characters.

Passing this test demonstrated that my logic accounted for real-world inputs rather than only ideal cases.

--- 

## Final Takeaway 
One of the biggest lessons from this module was learning to step back and look at the problem as a whole before diving into fixes. Adding debug output and walking through one test case at a time helped me understand what the program was actually doing, not just what I expected it to do.

This assignment also showed me the importance of thinking differently when edge cases are involved. The solution isn’t always about adding more code — sometimes it’s about changing how you approach the problem entirely.

Overall, this module pushed me to slow down, trust the fundamentals, and build solutions that are both correct and understandable.