[CmdletBinding(SupportsShouldProcess)]
param([ValidateSet('Debug','Release')][string]$Configuration='Release')
$ErrorActionPreference='Stop';$root=Split-Path -Parent $PSScriptRoot
$source=Join-Path $root "build/windows-x64/AuroraBroadcastProcessor_artefacts/$Configuration/VST3/Aurora Broadcast Processor.vst3"
$target=Join-Path $env:CommonProgramFiles 'VST3/Aurora Broadcast Processor.vst3'
if(-not(Test-Path $source)){throw "Built VST3 not found: $source"}
if($PSCmdlet.ShouldProcess($target,'Install development VST3')){if(Test-Path $target){throw 'Target already exists. Uninstall the development copy explicitly before replacing it.'};Copy-Item -LiteralPath $source -Destination $target -Recurse -Force;Set-Content -LiteralPath (Join-Path $target '.aurora-development-copy') -Value 'Created by install-vst3-local.ps1' -Encoding ASCII}

