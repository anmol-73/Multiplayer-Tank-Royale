@echo off
setlocal

:: Set the paths for the client and server folders
set "CLIENT_DIR=client"
set "SERVER_DIR=server"

:: Start the server make process in a new cmd instance
start cmd /k "cd /d %CLIENT_DIR% && make -j12"

:: Start the client make process in a new cmd instance
start cmd /k "cd /d %SERVER_DIR% && make -j12"

:: Wait for the client make process to complete
cd /d %CLIENT_DIR%
call make

:: Check if the executable exists before trying to run it
if exist MTR_CLIENT2 (
    start "" MTR_CLIENT2.exe
    start "" MTR_CLIENT2.exe
) else (
    echo MTR_CLIENT2 not found in %CLIENT_DIR%.
)

endlocal
