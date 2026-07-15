#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build="$root/build/macos-universal"
log="$build/test.log"
export AURORA_EXTENDED_TESTS="${AURORA_EXTENDED_TESTS:-0}"

set +e
ctest --test-dir "$build" -C Release --output-on-failure 2>&1 | tee "$log"
status=${PIPESTATUS[0]}
set -e

if (( status != 0 )); then
  diagnostics="$(tail -n 160 "$log")"
  diagnostics="${diagnostics//'%'/'%25'}"
  diagnostics="${diagnostics//$'\r'/'%0D'}"
  diagnostics="${diagnostics//$'\n'/'%0A'}"
  echo "::error title=macOS tests failed::$diagnostics"
  exit "$status"
fi
