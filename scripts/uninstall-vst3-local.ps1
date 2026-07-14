[CmdletBinding(SupportsShouldProcess)]param()
$ErrorActionPreference='Stop';$target=Join-Path $env:CommonProgramFiles 'VST3/Aurora Broadcast Processor.vst3';$marker=Join-Path $target '.aurora-development-copy'
if(-not(Test-Path -LiteralPath $marker)){throw 'Refusing removal: the development-install marker is absent.'}
$resolved=[IO.Path]::GetFullPath($target);$allowed=[IO.Path]::GetFullPath((Join-Path $env:CommonProgramFiles 'VST3'))
if(-not $resolved.StartsWith($allowed,[StringComparison]::OrdinalIgnoreCase)){throw 'Resolved target is outside the VST3 directory.'}
if($PSCmdlet.ShouldProcess($resolved,'Remove marked development VST3')){Remove-Item -LiteralPath $resolved -Recurse -Force}

