$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$WorkspaceDir = (Get-Item $ScriptDir).Parent.FullName

Push-Location $WorkspaceDir

New-Item -ItemType Directory -Force -Path "build" | Out-Null
New-Item -ItemType Directory -Force -Path "bin" | Out-Null

cmake -B build
cmake --build build --clean-first

Get-ChildItem -Path "$WorkspaceDir\bin\Debug\*" -Include "*.exe","*.pdb" | ForEach-Object {
    Copy-Item $_.FullName -Destination "$WorkspaceDir\bin\" -Force
}

Get-ChildItem -Path "$WorkspaceDir\bin\*.exe" | Select-Object -First 1 | ForEach-Object {
    & $_.FullName
}

Pop-Location
