# Configure, build, and test with the CMake that ships with Visual Studio.
#   .\build.ps1          build and run tests
#   .\build.ps1 -Run     ...then launch the sandbox
param([switch]$Run, [string]$Config = "Release")
# Not "Stop": PowerShell 5.1 treats any native stderr output (like CMake's
# warnings) as an error. Failures are caught with exit codes below instead.
$ErrorActionPreference = "Continue"

$vs = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
$cmake = Join-Path $vs "Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
$ctest = Join-Path (Split-Path $cmake) "ctest.exe"

Push-Location $PSScriptRoot
try {
    & $cmake -S . -B build
    if ($LASTEXITCODE) { throw "configure failed" }
    & $cmake --build build --config $Config
    if ($LASTEXITCODE) { throw "build failed" }
    & $ctest --test-dir build -C $Config --output-on-failure
    if ($LASTEXITCODE) { throw "tests failed" }
    if ($Run) { & ".\build\$Config\sandbox.exe" }
} finally {
    Pop-Location
}
