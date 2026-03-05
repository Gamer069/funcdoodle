#!/usr/bin/env sh
set -euo pipefail

find src -type f \( \
    -name "*.cc" -o \
    -name "*.cpp" -o \
    -name "*.c" -o \
    -name "*.h" -o \
    -name "*.hpp" \
\) -exec clang-format -i {} +

find test -type f \( \
    -name "*.cc" -o \
    -name "*.cpp" -o \
    -name "*.c" -o \
    -name "*.h" -o \
    -name "*.hpp" \
\) -exec clang-format -i {} +
