[CmdletBinding()]
param([switch]$Extended)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root 'build/windows-x64'
$log = Join-Path $build 'test.log'
$env:AURORA_EXTENDED_TESTS = if ($Extended) { '1' } else { '0' }

$output = @(& ctest --test-dir $build -C Release --output-on-failure 2>&1 | Tee-Object -FilePath $log)
$exitCode = $LASTEXITCODE
if ($exitCode -ne 0) {
    $diagnostics = @($output | Where-Object { "$_" -match '(?i)(AURORA_TEST_FAILURE|!!! Test|FAILED!!|tests failed|No tests were found|Unable to find executable)' } | Select-Object -Last 80)
    if ($diagnostics.Count -eq 0) { $diagnostics = @($output | Select-Object -Last 80) }
    $message = ($diagnostics -join "`n").Replace('%', '%25').Replace("`r", '%0D').Replace("`n", '%0A')
    Write-Host "::error title=Windows tests failed::$message"
    exit $exitCode
}
