#!/usr/bin/env bash
# makewords.sh: split whitespace-delimited words onto separate lines.

# -e: exit on error 
#-u: error on unset vars 
#pipefail: fail if any pipeline command fails
set -euo pipefail

# awk splits each input line into fields using whitespace
# print each field on its own line
awk '{for (i=1; i<=NF; i++) print $i}'