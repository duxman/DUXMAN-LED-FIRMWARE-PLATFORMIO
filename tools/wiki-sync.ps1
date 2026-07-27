[CmdletBinding()]
param(
    [ValidateSet('pull', 'push')]
    [string]$Mode = 'pull',

    [string]$WikiUrl,

    [string]$WikiBranch,

    [string]$LocalWikiPath = 'docs/wiki',

    [string]$WikiRepoPath = '.wiki-tmp',

    [string]$CommitMessage = "docs(wiki): sync from local mirror",

    [switch]$NoPush
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Write-Step {
    param([string]$Message)
    Write-Host "[wiki-sync] $Message"
}

function Invoke-Git {
    param(
        [Parameter(Mandatory = $true)][string[]]$Args,
        [string]$WorkingDirectory
    )

    $stdoutFile = [System.IO.Path]::GetTempFileName()
    $stderrFile = [System.IO.Path]::GetTempFileName()

    try {
        $startParams = @{
            FilePath               = 'git'
            ArgumentList           = $Args
            NoNewWindow            = $true
            Wait                   = $true
            PassThru               = $true
            RedirectStandardOutput = $stdoutFile
            RedirectStandardError  = $stderrFile
        }

        if ($WorkingDirectory) {
            $startParams.WorkingDirectory = $WorkingDirectory
        }

        $process = Start-Process @startParams
        $stdoutText = Get-Content -Path $stdoutFile -Raw -ErrorAction SilentlyContinue
        $stderrText = Get-Content -Path $stderrFile -Raw -ErrorAction SilentlyContinue
        if ($null -eq $stdoutText) { $stdoutText = '' }
        if ($null -eq $stderrText) { $stderrText = '' }
        $output = ($stdoutText + $stderrText).TrimEnd()
        $exitCode = $process.ExitCode
    }
    finally {
        Remove-Item -Path $stdoutFile -ErrorAction SilentlyContinue
        Remove-Item -Path $stderrFile -ErrorAction SilentlyContinue
    }

    if ($exitCode -ne 0) {
        throw "git $($Args -join ' ') failed.$([Environment]::NewLine)$output"
    }

    return $output
}

function Resolve-WikiUrl {
    param([string]$RepoRoot)

    $origin = (Invoke-Git -Args @('remote', 'get-url', 'origin') -WorkingDirectory $RepoRoot).Trim()

    if ([string]::IsNullOrWhiteSpace($origin)) {
        throw 'Could not resolve origin remote URL.'
    }

    if ($origin -match '^https://github\.com/(?<owner>[^/]+)/(?<repo>[^/.]+)(\.git)?$') {
        return "https://github.com/$($Matches.owner)/$($Matches.repo).wiki.git"
    }

    if ($origin -match '^git@github\.com:(?<owner>[^/]+)/(?<repo>[^/.]+)(\.git)?$') {
        return "https://github.com/$($Matches.owner)/$($Matches.repo).wiki.git"
    }

    throw "Unsupported origin format: $origin"
}

function Resolve-DefaultBranch {
    param([string]$WikiRepoDirectory)

    $headRef = (Invoke-Git -Args @('symbolic-ref', 'refs/remotes/origin/HEAD') -WorkingDirectory $WikiRepoDirectory).Trim()
    if ($headRef -match 'refs/remotes/origin/(?<branch>.+)$') {
        return $Matches.branch
    }

    $remoteBranches = Invoke-Git -Args @('branch', '-r') -WorkingDirectory $WikiRepoDirectory
    if ($remoteBranches -match 'origin/master') {
        return 'master'
    }

    return 'main'
}

function Mirror-Directory {
    param(
        [Parameter(Mandatory = $true)][string]$Source,
        [Parameter(Mandatory = $true)][string]$Destination,
        [string[]]$ExcludeDirectories = @(),
        [string[]]$ExcludeFiles = @()
    )

    if (-not (Test-Path -Path $Source -PathType Container)) {
        throw "Source path does not exist: $Source"
    }

    if (-not (Test-Path -Path $Destination -PathType Container)) {
        New-Item -ItemType Directory -Path $Destination | Out-Null
    }

    $arguments = @(
        $Source,
        $Destination,
        '/MIR',
        '/NFL',
        '/NDL',
        '/NJH',
        '/NJS',
        '/NP'
    )

    if ($ExcludeDirectories.Count -gt 0) {
        $arguments += '/XD'
        $arguments += $ExcludeDirectories
    }

    if ($ExcludeFiles.Count -gt 0) {
        $arguments += '/XF'
        $arguments += $ExcludeFiles
    }

    & robocopy @arguments | Out-Null
    $exitCode = $LASTEXITCODE

    if ($exitCode -ge 8) {
        throw "robocopy failed with exit code $exitCode"
    }
}

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw 'git is required but was not found in PATH.'
}

if (-not (Get-Command robocopy -ErrorAction SilentlyContinue)) {
    throw 'robocopy is required but was not found in PATH.'
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = (Resolve-Path (Join-Path $scriptRoot '..')).Path
$wikiMirrorPath = Join-Path $repoRoot $LocalWikiPath
$wikiRepoDirectory = Join-Path $repoRoot $WikiRepoPath

if (-not (Test-Path -Path $wikiMirrorPath -PathType Container)) {
    New-Item -ItemType Directory -Path $wikiMirrorPath | Out-Null
}

$wikiMirrorPath = (Resolve-Path $wikiMirrorPath).Path

if ([string]::IsNullOrWhiteSpace($WikiUrl)) {
    $WikiUrl = Resolve-WikiUrl -RepoRoot $repoRoot
}

Write-Step "Repository root: $repoRoot"
Write-Step "Wiki remote URL: $WikiUrl"
Write-Step "Wiki mirror folder: $wikiMirrorPath"
Write-Step "Wiki local repo cache: $wikiRepoDirectory"

if (-not (Test-Path -Path $wikiRepoDirectory -PathType Container)) {
    Write-Step 'Cloning wiki repository...'
    Invoke-Git -Args @('clone', $WikiUrl, $wikiRepoDirectory)
}
else {
    Write-Step 'Wiki repository already cloned. Fetching latest changes...'
    Invoke-Git -Args @('fetch', 'origin') -WorkingDirectory $wikiRepoDirectory
}

if ([string]::IsNullOrWhiteSpace($WikiBranch)) {
    $WikiBranch = Resolve-DefaultBranch -WikiRepoDirectory $wikiRepoDirectory
}

Write-Step "Using wiki branch: $WikiBranch"

Invoke-Git -Args @('checkout', $WikiBranch) -WorkingDirectory $wikiRepoDirectory
Invoke-Git -Args @('pull', '--ff-only', 'origin', $WikiBranch) -WorkingDirectory $wikiRepoDirectory

switch ($Mode) {
    'pull' {
        Write-Step 'Syncing wiki -> local docs mirror...'
        Mirror-Directory -Source $wikiRepoDirectory -Destination $wikiMirrorPath -ExcludeDirectories @('.git')
        Write-Step 'Done. Local docs mirror updated from wiki.'
    }

    'push' {
        Write-Step 'Syncing local docs mirror -> wiki repository...'
        Mirror-Directory -Source $wikiMirrorPath -Destination $wikiRepoDirectory -ExcludeDirectories @('.git')

        $status = Invoke-Git -Args @('status', '--porcelain') -WorkingDirectory $wikiRepoDirectory
        if (-not $status) {
            Write-Step 'No wiki changes to commit.'
            break
        }

        Invoke-Git -Args @('add', '--all') -WorkingDirectory $wikiRepoDirectory
        Invoke-Git -Args @('commit', '-m', $CommitMessage) -WorkingDirectory $wikiRepoDirectory

        if ($NoPush) {
            Write-Step 'Changes committed locally in wiki repo cache. Push skipped by -NoPush.'
        }
        else {
            Invoke-Git -Args @('push', 'origin', $WikiBranch) -WorkingDirectory $wikiRepoDirectory
            Write-Step 'Done. Local mirror changes pushed to GitHub wiki.'
        }
    }
}
