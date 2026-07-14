#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cmake --build "$root/build/macos-universal" \
  --config Release \
  --parallel "$(sysctl -n hw.logicalcpu)" \
  --target AuroraBroadcastProcessorTests AuroraBroadcastProcessor_VST3 AuroraBroadcastProcessor_AU AuroraBroadcastProcessor_Standalone
