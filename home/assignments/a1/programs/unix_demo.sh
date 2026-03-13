#!/usr/bin/env bash
# unix_demo.sh: run the full Unix-style text-processing pipeline.


# Pipeline stages:
# 1. sentence.sh   -> read input file
# 2. makewords.sh  -> split into one word per line
# 3. lowercase.sh  -> normalize to lowercase
# 4. sort.sh       -> sort words alphabetically
# 5. unique.sh     -> remove adjacent duplicates
# 6. mismatch.sh   -> remove words found in dictionary

# -e  exit immediately if a command fails
# -u  treat unset variables as errors
# -o pipefail  fail if any command in a pipeline fails
set -euo pipefail

# First argument: sentence file (defaults to unix_sentence.text)
SENT_FILE="${1:-unix_sentence.text}"

# Second argument: dictionary file (defaults to unix_dict.text)
DICT_FILE="${2:-unix_dict.text}"

# Each stage reads from stdin and writes to stdout
# The pipe operator connects the output of one stage directly into the input of the next stage.
./sentence.sh "$SENT_FILE" \
  | ./makewords.sh \
  | ./lowercase.sh \
  | ./sort.sh \
  | ./unique.sh \
  | ./mismatch.sh "$DICT_FILE"
