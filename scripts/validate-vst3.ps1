[CmdletBinding()]param([ValidateSet('Debug','Release')][string]$Configuration='Release')
$ErrorActionPreference='Stop';$root=Split-Path -Parent $PSScriptRoot;$output=Join-Path $root 'docs/vst3-validation.txt';$plugin=Join-Path $root "build/windows-x64/AuroraBroadcastProcessor_artefacts/$Configuration/VST3/Aurora Broadcast Processor.vst3"
if(-not(Test-Path $plugin)){"VALIDATION NOT RUN - BUILT VST3 UNAVAILABLE"|Set-Content $output -Encoding UTF8;Write-Warning 'Built VST3 unavailable.';exit 0}
$candidates=@((Get-Command validator.exe -ErrorAction SilentlyContinue).Source,(Join-Path $root 'tools/vst3-validator/validator.exe'))|Where-Object{$_ -and(Test-Path $_)}
if($candidates.Count-eq 0){"VALIDATION NOT RUN - VALIDATOR UNAVAILABLE"|Set-Content $output -Encoding UTF8;Write-Warning 'VALIDATION NOT RUN - VALIDATOR UNAVAILABLE';exit 0}
& $candidates[0] $plugin *>&1|Tee-Object -FilePath $output;$code=$LASTEXITCODE;if($code-ne 0){throw "VST3 validation failed with exit code $code"};Add-Content -LiteralPath $output -Value 'VALIDATION PASSED' -Encoding UTF8
