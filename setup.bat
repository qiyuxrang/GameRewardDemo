@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul 2>&1

echo ============================================
echo   GameRewardDemo - One-Click Setup
echo ============================================
echo.

:: ---- Check admin privileges for mklink /J ----
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo [INFO] Requesting admin privileges for junction link...
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)

:: ---- Detect script directory (project root) ----
set "PROJECT_DIR=%~dp0"
:: Remove trailing backslash
if "%PROJECT_DIR:~-1%"=="\" set "PROJECT_DIR=%PROJECT_DIR:~0,-1%"

echo [1/6] Checking project directory...
echo       %PROJECT_DIR%
echo.

:: ---- Check Visual Studio ----
echo [2/6] Checking Visual Studio...
set "VS_FOUND=0"

:: Check VS2022
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
    set "VS_EDITION=Community"
    set "VS_YEAR=2022"
    set "VS_FOUND=1"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" (
    set "VS_EDITION=Professional"
    set "VS_YEAR=2022"
    set "VS_FOUND=1"
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" (
    set "VS_EDITION=Enterprise"
    set "VS_YEAR=2022"
    set "VS_FOUND=1"
)

:: Check VS2019
if %VS_FOUND%==0 (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" (
        set "VS_EDITION=Community"
        set "VS_YEAR=2019"
        set "VS_FOUND=1"
    )
)
if %VS_FOUND%==0 (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\Tools\VsDevCmd.bat" (
        set "VS_EDITION=Professional"
        set "VS_YEAR=2019"
        set "VS_FOUND=1"
    )
)
if %VS_FOUND%==0 (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat" (
        set "VS_EDITION=Enterprise"
        set "VS_YEAR=2019"
        set "VS_FOUND=1"
    )
)

if %VS_FOUND%==0 (
    echo [ERROR] Visual Studio 2019 or 2022 not found!
    echo         Please install Visual Studio with "Desktop development with C++" workload.
    echo         Download: https://visualstudio.microsoft.com/downloads/
    pause
    exit /b 1
)

:: Determine VS generator
if "%VS_YEAR%"=="2022" (
    set "CMAKE_GEN=Visual Studio 17 2022"
) else (
    set "CMAKE_GEN=Visual Studio 16 2019"
)
echo       Found Visual Studio %VS_YEAR% %VS_EDITION%
echo       CMake generator: %CMAKE_GEN%
echo.

:: ---- Check CMake ----
echo [3/6] Checking CMake...
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found!
    echo         Please install CMake 3.16+ and add to PATH.
    echo         Download: https://cmake.org/download/
    pause
    exit /b 1
)
for /f "tokens=3" %%v in ('cmake --version 2^>nul ^| findstr /r "cmake version"') do (
    echo       CMake version: %%v
)
echo.

:: ---- Check Git ----
echo [4/6] Checking Git...
git --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Git not found! Required to clone cocos2d-x engine.
    echo         Download: https://git-scm.com/downloads
    pause
    exit /b 1
)
for /f "tokens=3" %%v in ('git --version 2^>nul') do (
    echo       %%v
)
echo.

:: ---- Check / Setup cocos2d-x engine ----
echo [5/6] Checking cocos2d-x engine...
set "COCOS_DIR=%PROJECT_DIR%\cocos2d"

if exist "%COCOS_DIR%\cocos\cocos2d.cpp" (
    echo       cocos2d-x engine already exists at: %COCOS_DIR%
) else (
    :: Check if it's a broken symlink or junction
    if exist "%COCOS_DIR%" (
        echo       Removing broken link...
        rmdir "%COCOS_DIR%" 2>nul
    )

    echo       cocos2d-x engine not found. Cloning from GitHub...
    echo       This may take a few minutes (shallow clone)...
    echo.
    git clone --depth 1 --branch v4 https://github.com/cocos2d/cocos2d-x.git "%COCOS_DIR%"
    if %errorlevel% neq 0 (
        echo [ERROR] Failed to clone cocos2d-x!
        echo         Check your internet connection and try again.
        echo         You can also manually clone:
        echo           git clone -b v4 https://github.com/cocos2d/cocos2d-x.git "%COCOS_DIR%"
        pause
        exit /b 1
    )
    echo       cocos2d-x v4 cloned successfully.
)
echo.

:: ---- Build ----
echo [6/6] Building project...
set "BUILD_DIR=%PROJECT_DIR%\build"

if exist "%BUILD_DIR%\GameRewardDemo.vcxproj" (
    echo       Existing build found, rebuilding...
) else (
    echo       Generating build files with CMake...
)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cd /d "%BUILD_DIR%"
cmake -G "%CMAKE_GEN%" -A Win32 ..
if %errorlevel% neq 0 (
    echo [ERROR] CMake generation failed!
    echo         Try deleting the build folder and running this script again.
    pause
    exit /b 1
)

echo.
echo       Compiling Release build...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    echo         Check the error messages above.
    pause
    exit /b 1
)

echo.
echo ============================================
echo   Build successful!
echo ============================================
echo.
set "EXE_PATH=%BUILD_DIR%\bin\GameRewardDemo\Release\GameRewardDemo.exe"
if exist "%EXE_PATH%" (
    echo   Executable: %EXE_PATH%
    echo.
    set /p "RUN=Launch game now? (Y/N): "
    if /i "!RUN!"=="Y" (
        start "" "%EXE_PATH%"
    )
) else (
    echo   Executable not found at expected location.
    echo   Check build output above for details.
)

echo.
pause
