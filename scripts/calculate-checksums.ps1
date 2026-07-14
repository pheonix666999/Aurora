[CmdletBinding()]param([Parameter(Mandatory)][string]$Version)
$ErrorActionPreference='Stop';$root=Split-Path -Parent $PSScriptRoot;$dist=Join-Path $root 'dist';$output=Join-Path $dist "AuroraBroadcastProcessor-Checksums-v$Version.txt"
$files=Get-ChildItem -LiteralPath $dist -File|Where-Object{$_.FullName-ne$output-and$_.Name-notmatch'TestResults|macOS'-and$_.Name-match"-v$([regex]::Escape($Version))\.(zip|exe|json)$"}|Sort-Object Name
if($files.Count-eq 0){throw 'No release assets found for checksums.'};$lines=@($files|ForEach-Object{"$((Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash.ToLowerInvariant())  $($_.Name)"});[IO.File]::WriteAllLines($output,$lines,[Text.UTF8Encoding]::new($false));Write-Host "Wrote $output"
