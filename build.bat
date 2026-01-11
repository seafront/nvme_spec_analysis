@echo off
setlocal

:: 스크립트 위치로 이동
cd /d "%~dp0"

echo ============================================
echo NVMe Library Build Script (Clang + MSVC)
echo ============================================
echo.

:: Visual Studio 2022 BuildTools 환경 설정
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if %ERRORLEVEL% NEQ 0 (
    echo Failed to setup MSVC environment!
    exit /b 1
)

:: 빌드 폴더 정리 및 생성
if exist build rmdir /s /q build
mkdir build

:: CMake Configure
cmake -S . -B build -G Ninja ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" ^
    -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang-cl.exe" ^
    -DCMAKE_LINKER="C:/Program Files/LLVM/bin/lld-link.exe" ^
    -DCMAKE_AR="C:/Program Files/LLVM/bin/llvm-lib.exe" ^
    -DBUILD_TESTS=ON

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo CMake configuration failed!
    exit /b 1
)

:: Build
cmake --build build --config Release

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build failed!
    exit /b 1
)

echo.
echo ============================================
echo Build completed successfully!
echo Output: build/libnvme.a
echo ============================================
echo.

endlocal
