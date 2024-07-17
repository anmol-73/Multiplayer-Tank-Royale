@echo off
mingw32-make clean
mingw32-make
cd build
game.exe
cd ../