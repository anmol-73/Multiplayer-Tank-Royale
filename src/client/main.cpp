#include <raylib.h>

#include "core/scene_management/scene_manager.hpp"
#include "pages/pages.hpp"

int main(int argc, char const *argv[])
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "git gud vro dhruv");
    SetWindowMinSize(800, 450);
    SetTargetFPS(60);

    SceneManagement::SceneManager::register_scene(SceneManagement::SceneName::MAIN_PAGE, new Pages::MainWindowScene());
    SceneManagement::SceneManager::init();

    SceneManagement::SceneManager::load_scene(SceneManagement::SceneName::MAIN_PAGE);   

    while (SceneManagement::SceneManager::is_active){
        SceneManagement::SceneManager::update();
    }

    SceneManagement::SceneManager::cleanup();
    return 0;
}


/**
 * # Makefile for da tank game
# Author: Me :)

PROJECT_NAME = MTR_CLIENT
MAIN = main.cpp # The entry point (relative to SRC_DIR)

# directories
SRC_DIR = ../../src/client/
COM_DIR = ../../src/common/
INC_DIR = ../../include/
LIB_DIR = ../../lib/
RAYLIB_DIR = C:/raylib/raylib/

# The compiler to use
COMPILER_PATH ?= C:/raylib/w64devkit/bin/# We have to use the gcc in raylib on windows! (Idk about other platforms)
CC = "${COMPILER_PATH}g++" # The compiler to use

# Compiler arguments
CFLAGS = -std=c++14
INCLUDE_DIRS = -I$(SRC_DIR) -I${INC_DIR} -I$(RAYLIB_DIR)src/ -I$(RAYLIB_DIR)src/external/"
LINK_DIRS = -L${LIB_DIR} -L$(RAYLIB_DIR)/src
LINK_LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
PLATFORM ?= PLATFORM_DESKTOP


# Recursive wildcard: https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard, $(abspath ${SRC_DIR}), *.cpp) # All .cpp files
OBJS = $(SRC:.cpp=.o) # Corresponding .o files

# NOTE: This is the default make target
# The project itself :)
$(PROJECT_NAME): $(OBJS)
	@echo Built all object files
	$(CC) -o $(PROJECT_NAME).exe $(OBJS) $(CFLAGS) $(INCLUDE_DIRS) $(LINK_DIRS) $(LINK_LIBS) -D$(PLATFORM)

# Build the object files
${OBJS}: %.o: %.cpp
	@echo Building "$<"
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_DIRS) -D$(PLATFORM)
# Delete all .o files in SRC_DIR and delete .exe file in build
clean:
	del ${PROJECT_NAME}.exe
	@echo Deleting object files
	@-for /f %%f in ('dir "${SRC_DIR}" /s /b ^| findstr /e .o') do @del %%f

 */