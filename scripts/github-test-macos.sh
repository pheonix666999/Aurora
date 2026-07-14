#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export AURORA_EXTENDED_TESTS="${AURORA_EXTENDED_TESTS:-0}"
ctest --test-dir "$root/build/macos-universal" -C Release --output-on-failure
