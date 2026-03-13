#!/usr/bin/env bash
set -euo pipefail

# Navigate to the script's directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${SCRIPT_DIR}"

### call the build script to compile all c code
./build.sh

### output to a text file using both programs
./readfile | ./linebreaker > output.text

### output to terminal using both programs
./readfile | ./linebreaker

