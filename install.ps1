# RupeeClarity one-line installer for Windows (PowerShell):
#   irm https://raw.githubusercontent.com/PrafullJ0shi/RupeeClarity/main/install.ps1 | iex
$ErrorActionPreference = 'Stop'
$url  = 'https://github.com/PrafullJ0shi/RupeeClarity/releases/latest/download/rupeeclarity-windows-x64.exe'
$dir  = Join-Path $env:LOCALAPPDATA 'RupeeClarity'
$exe  = Join-Path $dir 'rupeeclarity.exe'
New-Item -ItemType Directory -Force -Path $dir | Out-Null
Write-Host '==> Downloading RupeeClarity for Windows' -ForegroundColor Green
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
Invoke-WebRequest -Uri $url -OutFile $exe -UseBasicParsing
Unblock-File -Path $exe
Write-Host "==> Installed to $exe" -ForegroundColor Green
Write-Host '==> Starting RupeeClarity...' -ForegroundColor Green
Push-Location $dir
try { & $exe } finally { Pop-Location }
