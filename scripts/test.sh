#!/usr/bin/env bash

set -euo pipefail

usage() {
	echo "Usage: $0 [Debug|Release] [clean=true|false]"
	exit 1
}

if [[ $# -ne 0 ]] && [[ $# -ne 2 ]]; then
	usage
fi

mode="${1:-Debug}"
clean="${2:-false}"

mode_lower="$(echo "$mode" | tr '[:upper:]' '[:lower:]')"
if [[ "$mode_lower" != "debug" && "$mode_lower" != "release" ]]; then
	echo "Mode must be Debug or Release -- got '$mode'"
	exit 1
fi
mode="$(echo "$mode_lower" | awk '{print toupper(substr($0,1,1)) substr($0,2)}')"

clean_lower="$(echo "$clean" | tr '[:upper:]' '[:lower:]')"
if [[ "$clean_lower" != "true" && "$clean_lower" != "false" ]]; then
	echo "clean must be true or false -- got '$clean'"
	exit 1
fi

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if [[ "$clean_lower" == "true" ]]; then
	rm -rf "$root_dir/bin/linux"
fi

mkdir -p "$root_dir/bin/linux"
pushd "$root_dir/bin/linux" >/dev/null
cmake -DCMAKE_BUILD_TYPE="$mode" -DISTILING=ON -DBUILD_TESTS=ON "$root_dir"
jobs=$(( ($(nproc) + 2) / 3 ))
make -j"$jobs"
popd >/dev/null

ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=0}" \
LSAN_OPTIONS="${LSAN_OPTIONS:-detect_leaks=0}" \
"$root_dir/bin/linux/FuncDoodle"
