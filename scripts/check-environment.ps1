[CmdletBinding()]
param()
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$essentialMissing = @()

function Test-Tool([string]$Name, [switch]$Essential) {
    $command = Get-Command $Name -ErrorAction SilentlyContinue
    if ($command) { Write-Host "[FOUND] $Name - $($command.Source)" }
    else {
        Write-Warning "$Name was not found"
        if ($Essential) { $script:essentialMissing += $Name }
    }
}

Write-Host "Repository root resolved successfully."
Test-Tool git -Essential
Test-Tool cmake -Essential
Test-Tool cl -Essential
Test-Tool powershell -Essential
Test-Tool ninja
Test-Tool iscc
Test-Tool gh

if (-not (Test-Path (Join-Path $root 'VERSION.txt'))) { $essentialMissing += 'VERSION.txt' }
if ($essentialMissing.Count -gt 0) {
    Write-Error "Essential dependencies unavailable: $($essentialMissing -join ', ')"
    exit 1
}
Write-Host 'Environment is ready for configuration.'
exit 0

