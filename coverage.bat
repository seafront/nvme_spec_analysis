@echo off
setlocal

:: Script location
cd /d "%~dp0"

echo ============================================
echo NVMe Library Code Coverage Report
echo ============================================
echo.

:: Visual Studio 2022 BuildTools environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if %ERRORLEVEL% NEQ 0 (
    echo Failed to setup MSVC environment!
    exit /b 1
)

:: Clean and create build folder
if exist build rmdir /s /q build
mkdir build

:: CMake Configure with coverage enabled
cmake -S . -B build -G Ninja ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" ^
    -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" ^
    -DCMAKE_LINKER="C:/Program Files/LLVM/bin/lld-link.exe" ^
    -DCMAKE_AR="C:/Program Files/LLVM/bin/llvm-lib.exe" ^
    -DBUILD_TESTS=ON ^
    -DENABLE_COVERAGE=ON

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo CMake configuration failed!
    exit /b 1
)

:: Build
cmake --build build --config Debug

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build failed!
    exit /b 1
)

echo.
echo Running tests...
echo.

:: Set profile output file
set LLVM_PROFILE_FILE=build\coverage.profraw

:: Run tests
build\nvme_test.exe

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Tests failed!
    exit /b 1
)

echo.
echo Generating coverage report...
echo.

:: Merge profile data
"C:\Program Files\LLVM\bin\llvm-profdata.exe" merge -sparse build\coverage.profraw -o build\coverage.profdata

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Failed to merge profile data!
    exit /b 1
)

:: Create coverage directory
if not exist coverage mkdir coverage

:: Generate HTML report (source files only, ignore test files)
"C:\Program Files\LLVM\bin\llvm-cov.exe" show build\nvme_test.exe ^
    -instr-profile=build\coverage.profdata ^
    -format=html ^
    -output-dir=coverage ^
    -show-line-counts-or-regions ^
    -show-instantiations=false ^
    -ignore-filename-regex="test/.*" ^
    -ignore-filename-regex=".*gtest.*" ^
    -ignore-filename-regex=".*_deps.*" ^
    -Xdemangler="C:\Program Files\LLVM\bin\llvm-cxxfilt.exe"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Failed to generate HTML report!
    exit /b 1
)

:: Generate text summary
echo.
echo ============================================
echo Coverage Summary (src/NVMeDevice.cpp)
echo ============================================
"C:\Program Files\LLVM\bin\llvm-cov.exe" report build\nvme_test.exe ^
    -instr-profile=build\coverage.profdata ^
    -ignore-filename-regex="test/.*" ^
    -ignore-filename-regex=".*gtest.*" ^
    -ignore-filename-regex=".*_deps.*"

echo.
echo ============================================
echo Coverage report generated: coverage\index.html
echo ============================================
echo.

endlocal
