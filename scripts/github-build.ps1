[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root 'build/windows-x64'
$log = Join-Path $build 'build.log'
New-Item -ItemType Directory -Force $build | Out-Null

$output = @(& cmake --build $build --config Release --parallel 2>&1 | Tee-Object -FilePath $log)
$exitCode = $LASTEXITCODE
if ($exitCode -ne 0) {
    $diagnostics = @($output | Where-Object { "$_" -match '(?i)(fatal error|\berror\s+(C|LNK|MSB)\d+|: error:|undefined reference|unresolved external)' } | Select-Object -Last 80)
    if ($diagnostics.Count -eq 0) { $diagnostics = @($output | Select-Object -Last 120) }
    $message = ($diagnostics -join "`n").Replace('%', '%25').Replace("`r", '%0D').Replace("`n", '%0A')
    Write-Host "::error title=Windows native build failed::$message"
    exit $exitCode
}
