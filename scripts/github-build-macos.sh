#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build="$root/build/macos-universal"
log="$build/build.log"
mkdir -p "$build"

set +e
cmake --build "$build" \
  --config Release \
  --parallel "$(sysctl -n hw.logicalcpu)" \
  --target AuroraBroadcastProcessorTests AuroraBroadcastProcessor_VST3 AuroraBroadcastProcessor_AU AuroraBroadcastProcessor_Standalone \
  2>&1 | tee "$log"
status=${PIPESTATUS[0]}
set -e

if (( status != 0 )); then
  diagnostics="$(grep -Ei 'fatal error|: error:|undefined symbols|linker command failed' "$log" | tail -n 80 || true)"
  [[ -n "$diagnostics" ]] || diagnostics="$(tail -n 120 "$log")"
  diagnostics="${diagnostics//'%'/'%25'}"
  diagnostics="${diagnostics//$'\r'/'%0D'}"
  diagnostics="${diagnostics//$'\n'/'%0A'}"
  echo "::error title=macOS native build failed::$diagnostics"
  exit "$status"
fi
