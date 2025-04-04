$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$WorkspaceDir = (Get-Item $ScriptDir).Parent.FullName

Push-Location $WorkspaceDir

New-Item -ItemType Directory -Force -Path "build" | Out-Null
cmake -B build
cmake --build build --clean-first

& "$WorkspaceDir\bin\raylib_test.exe"

Pop-Location
