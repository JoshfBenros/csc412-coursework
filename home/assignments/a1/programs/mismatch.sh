#!/usr/bin/env bash
# mismatch.sh: print words from stdin that are not in the dictionary file.

# Approach:
# Load dictionary words into an associative array (used as a hash set)
# Stream stdin and print words that are not present in the dictionary set.


# -e  exit on error
# -u  error on unset variables
# -o pipefail  fail if any command in a pipeline fails
set -euo pipefail

# Use first argument as dictionary file - default to "unix_dict.text" if none provided
DICT_FILE="${1:-unix_dict.text}"

# Ensure dictionary file exists before continuing
if [[ ! -f "$DICT_FILE" ]]; then
  # Print error to stderr and exit if dictionary cannot be opened
  echo "mismatch.sh: could not open dictionary file: $DICT_FILE" >&2
  exit 1
fi

# Associative array acts as a hash set: keys = dictionary words, value is irrelevant (use 1)
declare -A dict

# Read dictionary file and insert each word into the associative array
while IFS= read -r w; do
  # Store word as a key in the hash
  dict["$w"]=1
done < "$DICT_FILE"

# stdin comes from the pipeline (sentence | makewords | lowercase | sort | unique).


# Stream stdin and print words not found in the dictionary set
while IFS= read -r word; do
  # If word is not present in the associative array, print it
  [[ -z "${dict[$word]+x}" ]] && printf '%s\n' "$word"
done
