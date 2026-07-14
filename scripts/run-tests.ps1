[CmdletBinding()]
param([switch]$Extended)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root 'build/windows-x64'
cmake --build $build --config Release --target AuroraBroadcastProcessorTests --parallel
$env:AURORA_EXTENDED_TESTS = if ($Extended) { '1' } else { '0' }
ctest --test-dir $build -C Release --output-on-failure
