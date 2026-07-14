#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
version="${1:-$(tr -d '[:space:]' < "$root/VERSION.txt")}"
expected_test_result="${2:-PASSED}"
dist="$root/dist"
artefacts="$root/build/macos-universal/AuroraBroadcastProcessor_artefacts/Release"
vst3="$artefacts/VST3/Aurora Broadcast Processor.vst3"
au="$artefacts/AU/Aurora Broadcast Processor.component"
standalone="$artefacts/Standalone/Aurora Broadcast Processor.app"

for bundle in "$vst3" "$au" "$standalone"; do
  [[ -d "$bundle" ]] || { echo "Missing bundle: $bundle" >&2; exit 1; }
  plutil -lint "$bundle/Contents/Info.plist" >/dev/null
  codesign --verify --deep --strict "$bundle"
  binary="$bundle/Contents/MacOS/Aurora Broadcast Processor"
  [[ -s "$binary" ]] || { echo "Missing bundle binary: $binary" >&2; exit 1; }
  archs="$(lipo -archs "$binary")"
  [[ " $archs " == *" arm64 "* && " $archs " == *" x86_64 "* ]] || {
    echo "Bundle is not Universal: $bundle ($archs)" >&2
    exit 1
  }
done

prefix="AuroraBroadcastProcessor"
required=(
  "$prefix-macOS-Universal-v$version.zip"
  "$prefix-VST3-macOS-Universal-v$version.zip"
  "$prefix-AU-macOS-Universal-v$version.zip"
  "$prefix-Standalone-macOS-Universal-v$version.zip"
  "$prefix-Setup-macOS-Universal-v$version.dmg"
  "$prefix-Checksums-macOS-Universal-v$version.txt"
  "$prefix-ReleaseManifest-macOS-Universal-v$version.json"
  "$prefix-TestResults-macOS-Universal-v$version.zip"
)

for name in "${required[@]}"; do
  [[ -s "$dist/$name" ]] || { echo "Missing or empty release output: $name" >&2; exit 1; }
done

for archive in "$dist"/*-macOS-Universal-v"$version".zip; do
  unzip -tq "$archive" >/dev/null
  if unzip -Z1 "$archive" | grep -Eiq '(^|/)(prompt\.txt|\.git|\.github|CMakeCache\.txt|Debug)(/|$)|\.(pfx|p12|pem|key|cer|crt|o|a|dSYM)(/|$)'; then
    echo "Prohibited content in $(basename "$archive")" >&2
    exit 1
  fi
done

combined="$dist/$prefix-macOS-Universal-v$version.zip"
for expected in \
  'VST3/Aurora Broadcast Processor.vst3/Contents/MacOS/Aurora Broadcast Processor' \
  'AU/Aurora Broadcast Processor.component/Contents/MacOS/Aurora Broadcast Processor' \
  'Standalone/Aurora Broadcast Processor.app/Contents/MacOS/Aurora Broadcast Processor' \
  'Documentation/README.md' 'Documentation/INSTALLATION.md' 'VERSION.txt' 'BUILD_INFO.json'; do
  unzip -Z1 "$combined" | grep -Fxq "$expected" || { echo "Combined ZIP missing $expected" >&2; exit 1; }
done

hdiutil verify "$dist/$prefix-Setup-macOS-Universal-v$version.dmg" >/dev/null
(
  cd "$dist"
  shasum -a 256 -c "$prefix-Checksums-macOS-Universal-v$version.txt"
)

python3 - "$dist/$prefix-ReleaseManifest-macOS-Universal-v$version.json" "$version" "$expected_test_result" <<'PY'
import json
import sys
from pathlib import Path

manifest = json.loads(Path(sys.argv[1]).read_text(encoding="utf-8"))
assert manifest["productVersion"] == sys.argv[2]
assert manifest["platform"] == "macOS"
assert manifest["architecture"] == "Universal (arm64, x86_64)"
assert manifest["pluginFormats"] == ["VST3", "AU", "Standalone"]
assert manifest["testResult"] == sys.argv[3]
assert manifest["codeSigningStatus"] == "AD_HOC"
assert manifest["notarizationStatus"] == "NOT_NOTARIZED"
PY

echo "macOS release output verification passed."
