import sys
import pathlib
import os
# Makefile for da tank game
# Author: Me :)
# Also fuck you dhruv


PROJECT_NAME = "MTR_SERVER"
MAIN = "main.cpp" # The entry point (relative to SRC_DIR)

# directories
SRC_DIR = r"..\..\src\server"
COM_DIR = r"..\..\src\common"
INC_DIR = r"..\..\include"
LIB_DIR = r"..\..\lib"
RAYLIB_DIR = r"C:\raylib\raylib"

# The compiler to use
CC = "g++" # The compiler to use

# Compiler arguments
LINK_LIBS = "-lraylib -lopengl32 -lgdi32 -lwinmm -lenet"
LINK_DIRS = f"-L{LIB_DIR} -L{RAYLIB_DIR}/src"
INCLUDE_DIRS = f"-I{SRC_DIR} -I{COM_DIR} -I{INC_DIR} -I{RAYLIB_DIR}/src/ -I{RAYLIB_DIR}/src/external/"
CFLAGS = "-Wall -std=c++23 -D_DEFAULT_SOURCE -Wno-missing-braces"
PLATFORM = "PLATFORM_DESKTOP"

if len(sys.argv) > 1 and (sys.argv[1] == "clean" or sys.argv[1] == "test"):
	print("Deleting object files")
	for file in pathlib.Path(SRC_DIR).glob("**/*.o"):
		file.unlink()

	exe = pathlib.Path(f"{PROJECT_NAME}.exe")
	if exe.exists():
		print("Deleting executable")
		exe.unlink()

# Get the files present in src
headers = [file.absolute().resolve().as_posix() for file in pathlib.Path(SRC_DIR).glob("**/*.hpp")] + [file.absolute().resolve().as_posix() for file in pathlib.Path(COM_DIR).glob("**/*.hpp")]
sources = [file.absolute().resolve().as_posix() for file in pathlib.Path(SRC_DIR).glob("**/*.cpp")] + [file.absolute().resolve().as_posix() for file in pathlib.Path(COM_DIR).glob("**/*.cpp")]
objects = [file.removesuffix(".cpp") + ".o" for file in sources]

OBJS = " ".join(f"\"{object}\"" for object in objects)
HEADERS = " ".join(f"\"{header}\"" for header in headers)

for source, object in zip(sources, objects):
	print(f"Building \"{source}\"")
	assert os.system(
		f"{CC} -c \"{source}\" -o \"{object}\" {CFLAGS} {INCLUDE_DIRS} -D{PLATFORM}"
    ) == 0

print("Built all object files")
print("Building executable")
assert os.system(
	f"{CC} -o {PROJECT_NAME}.exe {OBJS} {HEADERS} {CFLAGS} {INCLUDE_DIRS} {LINK_DIRS} {LINK_LIBS} -D{PLATFORM}"
) == 0
print("Built executabe")


if len(sys.argv) > 1 and sys.argv[1] == "test":
	assert os.system(
		f"{PROJECT_NAME}.exe"
	) == 0
