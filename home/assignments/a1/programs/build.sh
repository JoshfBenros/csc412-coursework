#!/usr/bin/env bash

# Compile all C programs in this directory
# If a program includes fio.h, automatically locate the fio directory and link its C source files during compilation

# -e  : exit immediately if a command fails
# -u  : treat unset variables as errors
# -o pipefail : fail if any command in a pipeline fails
set -euo pipefail


# Compiler and flags
# -Wall -Wextra : enable warnings
# -std=c11      : use C11 standard
# -O2           : optimize for speed
CC=gcc
CFLAGS="-Wall -Wextra -std=c11 -O2"

# Determine directory where build.sh lives and switch to it
# This ensures compilation works no matter where script is called from
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"


# Search parent directories for a folder named "fio"
# Needed for programs that include "../fio/fio.h"
FIO_DIR="$(find "$SCRIPT_DIR/.." -name 'fio' -type d -print -quit || true)"

# Enable nullglob so *.c expands to empty if no matches
shopt -s nullglob
C_FILES=( *.c )
shopt -u nullglob

# Fail early if no C source files found
if [ ${#C_FILES[@]} -eq 0 ]; then
  echo "build.sh: no C files found in ${SCRIPT_DIR}" >&2
  exit 1
fi

# For each C file:
# Determine output executable name
# Check if file depends on fio.h
# If yes, compile and link with fio sources
# Otherwise compile normally
for c in "${C_FILES[@]}"; do
  exe="${c%.c}"

  if grep -qE '^[[:space:]]*#include[[:space:]]+".*fio\.h"' "$c"; then
    if [[ -z "$FIO_DIR" ]]; then
      echo "build.sh: could not locate fio directory (needed for ${c})" >&2
      exit 1
    fi

    # Gather all C files inside fio directory
    FIO_C_FILES=( "$FIO_DIR"/*.c )
    if [ ${#FIO_C_FILES[@]} -eq 0 ]; then
      echo "build.sh: no fio C files found in ${FIO_DIR}" >&2
      exit 1
    fi
    # Compile with include path and link fio sources
    $CC $CFLAGS -I"$FIO_DIR" -o "$exe" "$c" "${FIO_C_FILES[@]}"
  else
  # Standard compilation for standalone programs
    $CC $CFLAGS -o "$exe" "$c"
  fi
done

# Ensure all shell scripts in this directory are executable
chmod +x ./*.sh
