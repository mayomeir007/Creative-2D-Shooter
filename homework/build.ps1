param(
    [ValidateSet("Debug", "Release", "RelWithDebInfo", "MinSizeRel")]
    [string]$Config = "Release",
    [switch]$Run
)

$ErrorActionPreference = "Stop"

$cmakeCommand = Get-Command cmake -ErrorAction SilentlyContinue
if ($cmakeCommand) {
    $cmakeExe = $cmakeCommand.Source
}
else {
    $cmakeCandidates = @(
        "${env:ProgramFiles}\CMake\bin\cmake.exe",
        "${env:ProgramFiles(x86)}\CMake\bin\cmake.exe"
    )

    $cmakeExe = $cmakeCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if (-not $cmakeExe) {
        Write-Error "CMake is not installed or not on PATH. Install CMake and retry."
    }
}

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildDir = Join-Path $root "build"

& $cmakeExe -S $root -B $buildDir
& $cmakeExe --build $buildDir --config $Config

if ($Run) {
    $multiConfigExe = Join-Path $buildDir "$Config/homework-assignment.exe"
    $singleConfigExe = Join-Path $buildDir "homework-assignment.exe"

    if (Test-Path $multiConfigExe) {
        & $multiConfigExe
    }
    elseif (Test-Path $singleConfigExe) {
        & $singleConfigExe
    }
    else {
        Write-Error "Build completed, but executable was not found in expected locations."
    }
}
