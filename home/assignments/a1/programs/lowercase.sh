#!/usr/bin/env bash
# lowercase.sh: convert stdin to lowercase using tr.

# -e  : exit immediately if a command fails
# -u  : treat unset variables as errors
# -o pipefail : fail if any command in a pipeline fails
set -euo pipefail

# LC_ALL=C forces byte-wise ASCII behavior
# tr maps characters in 'A-Z' to 'a-z'
LC_ALL=C tr 'A-Z' 'a-z'