#!/usr/bin/env bash
set -euo pipefail

# 1) Build project (to have compile_commands.json)
BUILD_DIR="${1:-build}"
mkdir -p "$BUILD_DIR"
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build "$BUILD_DIR" -j

# 2) Run clang-tidy with auto-fix
# 	--fix executes patches, --format-style=file uses your .clang-format (if found)
# 	--header-filter is 
clang-tidy -p "$BUILD_DIR" \
    --fix \
    --format-style=file \
    --header-filter='^(src|include)/.*' \
    $(git ls-files '*.cpp' '*.cc' '*.cxx' '*.c' '*.hpp' '*.hh' '*.h' '*.hxx')