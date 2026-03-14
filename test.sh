#!/usr/bin/env bash
# Run all xjb tests.
# Usage:
#   ./test.sh              (build + test)
#   ./test.sh --build-only (only build, no test)
#

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "=== Configuring ==="
cmake -S "${SCRIPT_DIR}/test" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release

echo "=== Building ==="
cmake --build "${BUILD_DIR}" -j"$(nproc)"

if [[ "${1:-}" == "--build-only" ]]; then
  echo "Build succeeded. Skipping tests (--build-only)."
  exit 0
fi

echo "=== Running tests ==="
ctest --test-dir "${BUILD_DIR}" --output-on-failure
