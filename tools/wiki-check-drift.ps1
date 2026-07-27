[CmdletBinding()]
param(
    [string]$WikiUrl
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = (Resolve-Path (Join-Path $scriptRoot '..')).Path

Write-Host '[wiki-check] Running wiki pull sync...'

$syncArgs = @(
    '-NoProfile',
    '-ExecutionPolicy',
    'Bypass',
    '-File',
    (Join-Path $scriptRoot 'wiki-sync.ps1'),
    '-Mode',
    'pull'
)

if (-not [string]::IsNullOrWhiteSpace($WikiUrl)) {
    $syncArgs += @('-WikiUrl', $WikiUrl)
}

& powershell @syncArgs
if ($LASTEXITCODE -ne 0) {
    throw "wiki-sync.ps1 failed with exit code $LASTEXITCODE"
}

Push-Location $repoRoot
try {
    $drift = git status --porcelain -- docs/wiki
    if ($LASTEXITCODE -ne 0) {
        throw 'git status failed while checking docs/wiki drift.'
    }

    if ([string]::IsNullOrWhiteSpace(($drift | Out-String))) {
        Write-Host '[wiki-check] OK: docs/wiki is synchronized with remote wiki.'
        exit 0
    }

    Write-Host '[wiki-check] ERROR: docs/wiki is out of sync with remote wiki.'
    Write-Host '[wiki-check] Run: powershell -ExecutionPolicy Bypass -File .\tools\wiki-sync.ps1 -Mode pull'
    Write-Host '[wiki-check] Changed files:'
    $drift | ForEach-Object { Write-Host "  $_" }
    exit 1
}
finally {
    Pop-Location
}
