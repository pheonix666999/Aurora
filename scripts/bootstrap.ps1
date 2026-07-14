[CmdletBinding()]
param()
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
& (Join-Path $PSScriptRoot 'check-environment.ps1')
cmake -S $root -B (Join-Path $root 'build/windows-x64') -G 'Visual Studio 17 2022' -A x64

