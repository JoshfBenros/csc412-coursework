#!/usr/bin/env bash
# sentence.sh: print the sentence file (defaults to unix_sentence.text).

# -e  exit immediately if a command fails
# -u  error on unset variables
# -o pipefail  fail if any command in a pipeline fails
set -euo pipefail

# Use first argument as the sentence file - default to "unix_sentence.text"
SENT_FILE="${1:-unix_sentence.text}"

# Ensure the file exists before attempting to print it
if [[ ! -f "$SENT_FILE" ]]; then
  echo "sentence.sh: could not open sentence file: $SENT_FILE" >&2
  exit 1
fi

# Stream file contents to stdout 
cat "$SENT_FILE"