# Makefile for da tank game
# Author: Me :)

PROJECT_NAME = MTR_CLIENT
MAIN = main.cpp # The entry point (relative to SRC_DIR)

# directories
SRC_DIR = ../../src/client
COM_DIR = ../../src/common
INC_DIR = ../../include
LIB_DIR = ../../lib
RAYLIB_DIR = C:/raylib/raylib

# The compiler to use
# We have to use the gcc in raylib on windows! (Idk about other platforms)
COMPILER_PATH ?= C:/raylib/w64devkit/bin
CC = g++ # The compiler to use

# Compiler arguments
CFLAGS = -Wall -std=c++14 -D_DEFAULT_SOURCE -Wno-missing-braces
INCLUDE_DIRS = -I$(SRC_DIR) -I${INC_DIR} -I$(RAYLIB_DIR)/src/ -I$(RAYLIB_DIR)/src/external/
LINK_DIRS = -L${LIB_DIR} -L$(RAYLIB_DIR)/src
LINK_LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
PLATFORM ?= PLATFORM_DESKTOP

# Recursive wildcard: https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard, $(abspath ${SRC_DIR}), *.cpp) # All .cpp files
HEADERS = $(call rwildcard, $(abspath ${SRC_DIR}), *.hpp) # All .hpp files
OBJS = $(SRC:.cpp=.o) # Corresponding .o files

# NOTE: This is the default make target
# The project itself :)
$(PROJECT_NAME): $(OBJS)
	@echo Built all object files
	@echo Building executable
	@$(CC) -o $(PROJECT_NAME).exe $(OBJS) $(HEADERS) $(CFLAGS) $(INCLUDE_DIRS) $(LINK_DIRS) $(LINK_LIBS) -D$(PLATFORM)
	@echo Built executable $(PROJECT_NAME)

# Build the object files
${OBJS}: %.o: %.cpp
	@echo Building "$<"
	@$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_DIRS) -D$(PLATFORM)

# Delete all .o files in SRC_DIR and delete .exe file in build
clean:
	del ${PROJECT_NAME}.exe
	@echo Deleting object files
	@-for /f %%f in ('dir "${SRC_DIR}" /s /b ^| findstr /e .o') do @del %%f
