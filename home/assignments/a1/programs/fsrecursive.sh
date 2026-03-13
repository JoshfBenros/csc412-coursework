#!/usr/bin/env bash
# Placeholder script for the fsrecursive step in the Unix demo.

set -euo pipefail

if [[ ! -x ./fsrecursive ]]; then
    echo "fsrecursive.sh: ./fsrecursive not found or not executable (run ./build.sh)" >&2
    exit 1
fi

./fsrecursive \
| ./makewords \
| ./lowercase \
| ./sort \
| ./unique \
| ./mismatch unix_dict.text \
> fsrecursion_unique_output.text