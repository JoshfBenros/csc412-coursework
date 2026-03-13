#!/usr/bin/env bash
# sort.sh: sort lines from stdin using locale-independent ordering.

# -e  exit immediately if a command fails
# -u  treat unset variables as errors
# -o pipefail  fail if any command in a pipeline fails
set -euo pipefail

# LC_ALL=C ensures predictable, locale-independent sorting
LC_ALL=C sort