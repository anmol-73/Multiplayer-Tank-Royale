# Makefile for da tank game
# Author: Me :)

PROJECT_NAME = client
MAIN = main.cpp # The entry point (relative to SRC_DIR)

# directories
SRC_DIR = ../../src/client
COM_DIR = ../../src/common
INC_DIR = ../../include
LIB_DIR = ../../lib
RAYLIB_DIR = C:/raylib/raylib

# The compiler to use
CC = g++ # The compiler to use

# Compiler arguments
LINK_LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -lenet
LINK_DIRS = -L${LIB_DIR} -L$(RAYLIB_DIR)/src
INCLUDE_DIRS = -I$(SRC_DIR) -I${INC_DIR} -I${COM_DIR} -I$(RAYLIB_DIR)/src/ -I$(RAYLIB_DIR)/src/external/
CFLAGS = -Wall -std=c++23 -D_DEFAULT_SOURCE -Wno-missing-braces
PLATFORM ?= PLATFORM_DESKTOP

# Recursive wildcard: https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Get the files present in src
SRC = $(call rwildcard, $(abspath ${SRC_DIR}), *.cpp) # All .cpp files
SRC += $(call rwildcard, $(abspath ${COM_DIR}), *.cpp)

OBJS = $(SRC:.cpp=.o) # Corresponding .o files
HEADERS = $(call rwildcard, $(abspath ${SRC_DIR}), *.hpp) # All .hpp files
HEADERS += $(call rwildcard, $(abspath ${COM_DIR}), *.hpp) # All .hpp files

GHEADERS = $(HEADERS:=.gch)

# NOTE: This is the default make target
# Build the executable
$(PROJECT_NAME): $(GHEADERS) $(OBJS)
	@echo Built all object files
	@echo Building executable
	@$(CC) -o $(PROJECT_NAME).exe $(OBJS) $(CFLAGS) $(INCLUDE_DIRS) $(LINK_DIRS) $(LINK_LIBS) -D$(PLATFORM)
	@echo Built executable $(PROJECT_NAME)

# Build the object files
${OBJS}: %.o: %.cpp
	@echo Building "$<"
	@$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_DIRS) -D$(PLATFORM)

${GHEADERS}: %.hpp.gch: %.hpp
	@echo Prcompiling "$<"
	@$(CC) $< $(CFLAGS) $(INCLUDE_DIRS) -D$(PLATFORM)

# Delete all .o files in SRC_DIR and delete .exe file in build
clean:
	del ${PROJECT_NAME}.exe
	@echo Deleting object files
	@-for /f %%f in ('dir "${SRC_DIR}" /s /b ^| findstr /e .o') do @del %%f
	@-for /f %%f in ('dir "${COM_DIR}" /s /b ^| findstr /e .o') do @del %%f

# Delete all .o files in SRC_DIR and delete .exe file in build
purge:
	del ${PROJECT_NAME}.exe
	@echo Deleting object files
	@-for /f %%f in ('dir "${SRC_DIR}" /s /b ^| findstr /e .gch') do @del %%f
	@-for /f %%f in ('dir "${COM_DIR}" /s /b ^| findstr /e .gch') do @del %%f
	@-for /f %%f in ('dir "${SRC_DIR}" /s /b ^| findstr /e .o') do @del %%f
	@-for /f %%f in ('dir "${COM_DIR}" /s /b ^| findstr /e .o') do @del %%f

# Build the project and run the executable
test:
	@${MAKE} ${PROJECT_NAME}
	@echo Running ${PROJECT_NAME}
	@${PROJECT_NAME}
