#!/usr/bin/env bash
# unique.sh: print only lines that differ from the previous line.

# -e  exit immediately if a command fails
# -u  treat unset variables as errors
# -o pipefail  fail if any command in a pipeline fails
set -euo pipefail

# NR==1 -> For the first line store it in `prev`, print it, and skip to next line
# $0 != prev -> For all other lines if current line differs from previous line, print it and update `prev`
awk '
NR==1 { prev=$0; print; next }
$0 != prev { print; prev=$0 }
'
