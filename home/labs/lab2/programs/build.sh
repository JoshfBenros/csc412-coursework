#! /usr/bin/env bash
set -euo pipefail
### compile all c code in the current directory

### Hints:
### how do you get the current directory in bash?
### how do you store the output of variables in bash?


# store current directory and fio directory
CURRENT_DIR="$(pwd)"

# Locate fio relative to THIS script (more robust than using pwd)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
FIO_DIR="${SCRIPT_DIR}/../fio"

# Change to the script's directory to ensure we find the C files
cd "${SCRIPT_DIR}"

# find C files
shopt -s nullglob
C_FILES=( *.c )
shopt -u nullglob

# if no C file, error message
if [ ${#C_FILES[@]} -eq 0 ]; then 
    echo "Oops, we found no C code in the directory: ${SCRIPT_DIR}" >&2
    exit 1
fi

# compile all programs in this directory
for c in "${C_FILES[@]}"; do
  exe="${c%.c}"

  # If this file includes fio.h, compile and link all C files from fio directory
  if grep -qE '^[[:space:]]*#include[[:space:]]+".*fio\.h"' "$c"; then
    FIO_C_FILES=( "${FIO_DIR}"/*.c )
    gcc -Wall -I"${FIO_DIR}" -o "${exe}" "$c" "${FIO_C_FILES[@]}"
  else
    gcc -Wall -o "${exe}" "$c"
  fi
done
