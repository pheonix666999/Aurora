[CmdletBinding()]
param()
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
cmake --build (Join-Path $root 'build/windows-x64') --config Release --parallel

