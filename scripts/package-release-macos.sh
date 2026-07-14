#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
version="${1:-$(tr -d '[:space:]' < "$root/VERSION.txt")}"
test_result="${2:-UNKNOWN}"
[[ "$version" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]] || { echo "Version must be semantic x.y.z" >&2; exit 1; }
[[ "$test_result" == "PASSED" || "$test_result" == "FAILED" || "$test_result" == "UNKNOWN" ]] || { echo "Invalid test result" >&2; exit 1; }
[[ "$(tr -d '[:space:]' < "$root/VERSION.txt")" == "$version" ]] || { echo "VERSION.txt mismatch" >&2; exit 1; }

dist="$root/dist"
stage="$dist/macos-staging"
artefacts="$root/build/macos-universal/AuroraBroadcastProcessor_artefacts/Release"
vst3="$artefacts/VST3/Aurora Broadcast Processor.vst3"
au="$artefacts/AU/Aurora Broadcast Processor.component"
standalone="$artefacts/Standalone/Aurora Broadcast Processor.app"

for bundle in "$vst3" "$au" "$standalone"; do
  [[ -d "$bundle" ]] || { echo "Missing bundle: $bundle" >&2; exit 1; }
  binary="$bundle/Contents/MacOS/Aurora Broadcast Processor"
  [[ -s "$binary" ]] || { echo "Missing executable: $binary" >&2; exit 1; }
  archs="$(lipo -archs "$binary")"
  [[ " $archs " == *" arm64 "* && " $archs " == *" x86_64 "* ]] || { echo "Not Universal: $bundle ($archs)" >&2; exit 1; }
  codesign --force --deep --sign - "$bundle"
  codesign --verify --deep --strict "$bundle"
done

mkdir -p "$dist"
case "$stage" in "$root/dist/"*) rm -rf "$stage" ;; *) echo "Unsafe staging path" >&2; exit 1 ;; esac
mkdir -p "$stage"

copy_common() {
  local destination="$1"
  mkdir -p "$destination"
  cp "$root/LICENSE_NOTICE.md" "$root/THIRD_PARTY_NOTICES.md" "$root/VERSION.txt" "$destination/"
}

combined="$stage/combined"
copy_common "$combined"
mkdir -p "$combined/VST3" "$combined/AU" "$combined/Standalone" "$combined/Documentation"
cp -R "$vst3" "$combined/VST3/"
cp -R "$au" "$combined/AU/"
cp -R "$standalone" "$combined/Standalone/"
cp "$root/README.md" "$combined/Documentation/README.md"
for document in INSTALLATION.md HOST_TESTING.md PRESETS.md KNOWN_LIMITATIONS.md; do
  cp "$root/docs/$document" "$combined/Documentation/$document"
done

BUILD_INFO_PATH="$combined/BUILD_INFO.json" VERSION="$version" GITHUB_SHA_VALUE="${GITHUB_SHA:-UNCOMMITTED}" python3 <<'PY'
import json
import os
from datetime import datetime, timezone
from pathlib import Path

info = {
    "product": "Aurora Broadcast Processor",
    "version": os.environ["VERSION"],
    "commit": os.environ["GITHUB_SHA_VALUE"],
    "juce": "8.0.14",
    "configuration": "Release",
    "platform": "macOS",
    "architecture": "Universal (arm64, x86_64)",
    "formats": ["VST3", "AU", "Standalone"],
    "codeSigning": "AD_HOC",
    "notarization": "NOT_NOTARIZED",
    "timestampUtc": datetime.now(timezone.utc).isoformat(),
}
Path(os.environ["BUILD_INFO_PATH"]).write_text(json.dumps(info, indent=2) + "\n", encoding="utf-8")
PY

vst3_stage="$stage/vst3"
copy_common "$vst3_stage"
cp -R "$vst3" "$vst3_stage/"
cp "$root/docs/INSTALLATION.md" "$vst3_stage/"

au_stage="$stage/au"
copy_common "$au_stage"
cp -R "$au" "$au_stage/"
cp "$root/docs/INSTALLATION.md" "$au_stage/"

standalone_stage="$stage/standalone"
copy_common "$standalone_stage"
cp -R "$standalone" "$standalone_stage/"
cp "$root/docs/INSTALLATION.md" "$standalone_stage/"

prefix="AuroraBroadcastProcessor"
python3 "$root/scripts/create-deterministic-zip.py" "$combined" "$dist/$prefix-macOS-Universal-v$version.zip"
python3 "$root/scripts/create-deterministic-zip.py" "$vst3_stage" "$dist/$prefix-VST3-macOS-Universal-v$version.zip"
python3 "$root/scripts/create-deterministic-zip.py" "$au_stage" "$dist/$prefix-AU-macOS-Universal-v$version.zip"
python3 "$root/scripts/create-deterministic-zip.py" "$standalone_stage" "$dist/$prefix-Standalone-macOS-Universal-v$version.zip"

dmg_stage="$stage/dmg"
mkdir -p "$dmg_stage"
cp -R "$vst3" "$au" "$standalone" "$dmg_stage/"
cp "$root/docs/INSTALLATION.md" "$dmg_stage/READ ME - INSTALLATION.md"
ln -s /Library/Audio/Plug-Ins/VST3 "$dmg_stage/VST3 Plug-Ins"
ln -s /Library/Audio/Plug-Ins/Components "$dmg_stage/Audio Unit Components"
ln -s /Applications "$dmg_stage/Applications"
dmg="$dist/$prefix-Setup-macOS-Universal-v$version.dmg"
rm -f "$dmg"
hdiutil create -quiet -volname "AuroraBP $version" -srcfolder "$dmg_stage" -ov -format UDZO "$dmg"

test_stage="$stage/tests"
mkdir -p "$test_stage"
cp "$root/docs/BUILD_LOG.md" "$test_stage/"
[[ -d "$root/build/macos-universal/Testing" ]] && cp -R "$root/build/macos-universal/Testing" "$test_stage/"
python3 "$root/scripts/create-deterministic-zip.py" "$test_stage" "$dist/$prefix-TestResults-macOS-Universal-v$version.zip"

MANIFEST_PATH="$dist/$prefix-ReleaseManifest-macOS-Universal-v$version.json" DIST_PATH="$dist" VERSION="$version" TEST_RESULT="$test_result" python3 <<'PY'
import hashlib
import json
import os
import platform
import subprocess
from datetime import datetime, timezone
from pathlib import Path

dist = Path(os.environ["DIST_PATH"])
version = os.environ["VERSION"]
public = sorted(
    path for path in dist.iterdir()
    if path.is_file()
    and f"v{version}" in path.name
    and "macOS-Universal" in path.name
    and "Checksums" not in path.name
    and "ReleaseManifest" not in path.name
    and "TestResults" not in path.name
)

def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()

manifest = {
    "schemaVersion": 1,
    "productName": "Aurora Broadcast Processor",
    "productVersion": version,
    "companyName": "Ammar Audio Labs",
    "platform": "macOS",
    "architecture": "Universal (arm64, x86_64)",
    "deploymentTarget": "macOS 11.0",
    "gitCommit": os.environ.get("GITHUB_SHA", "UNCOMMITTED"),
    "gitTag": os.environ.get("GITHUB_REF_NAME", "NONE") if os.environ.get("GITHUB_REF_TYPE") == "tag" else "NONE",
    "workflowRunId": os.environ.get("GITHUB_RUN_ID", "LOCAL"),
    "workflowRunNumber": os.environ.get("GITHUB_RUN_NUMBER", "LOCAL"),
    "buildTimestampUtc": datetime.now(timezone.utc).isoformat(),
    "runnerImage": os.environ.get("ImageOS", platform.platform()),
    "cmakeVersion": subprocess.check_output(["cmake", "--version"], text=True).splitlines()[0].removeprefix("cmake version "),
    "compilerVersion": subprocess.check_output(["xcodebuild", "-version"], text=True).strip().replace("\n", " / "),
    "juceVersion": "8.0.14",
    "juceCommit": "2cdfca8feb300fb424002ba2c2751569e5bacb64",
    "buildConfiguration": "Release",
    "pluginFormats": ["VST3", "AU", "Standalone"],
    "testsExecuted": ["parameter", "state", "preset", "DSP", "crossover", "stereo", "limiter", "lifecycle", "stress"],
    "testResult": os.environ["TEST_RESULT"],
    "bundleValidationStatus": "PASSED",
    "codeSigningStatus": "AD_HOC",
    "notarizationStatus": "NOT_NOTARIZED",
    "installerStatus": "DMG_BUILT",
    "repeatability": "Repeatable clean CI build",
    "outputs": [{"file": path.name, "sizeBytes": path.stat().st_size, "sha256": sha256(path)} for path in public],
}
Path(os.environ["MANIFEST_PATH"]).write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
PY

checksum="$dist/$prefix-Checksums-macOS-Universal-v$version.txt"
(
  cd "$dist"
  shasum -a 256 \
    "$prefix-macOS-Universal-v$version.zip" \
    "$prefix-VST3-macOS-Universal-v$version.zip" \
    "$prefix-AU-macOS-Universal-v$version.zip" \
    "$prefix-Standalone-macOS-Universal-v$version.zip" \
    "$prefix-Setup-macOS-Universal-v$version.dmg" \
    "$prefix-ReleaseManifest-macOS-Universal-v$version.json" \
    | LC_ALL=C sort > "$checksum"
)

"$root/scripts/verify-release-output-macos.sh" "$version" "$test_result"
