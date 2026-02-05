#OBJS specifies which files to compile as part of the project
OBJS = main.c

#CC specifies which compiler we're using
# Use clang on macOS for better SDK compatibility
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CC = clang
else
    CC = gcc
endif

# Detect OS and set SDL2 paths accordingly
ifeq ($(UNAME_S),Darwin)
    # macOS - use Homebrew paths
    # Include both the base include dir (for <SDL2/SDL.h>) and SDL2 subdir (for "SDL.h")
    SDL2_PREFIX := $(shell brew --prefix sdl2 2>/dev/null)
    SDL2_IMAGE_PREFIX := $(shell brew --prefix sdl2_image 2>/dev/null)
    SDL2_TTF_PREFIX := $(shell brew --prefix sdl2_ttf 2>/dev/null)
    SDL2_MIXER_PREFIX := $(shell brew --prefix sdl2_mixer 2>/dev/null)
    INCLUDE_FLAGS = -I$(SDL2_PREFIX)/include -I$(SDL2_PREFIX)/include/SDL2 -I$(SDL2_IMAGE_PREFIX)/include -I$(SDL2_IMAGE_PREFIX)/include/SDL2 -I$(SDL2_TTF_PREFIX)/include -I$(SDL2_TTF_PREFIX)/include/SDL2 -I$(SDL2_MIXER_PREFIX)/include -I$(SDL2_MIXER_PREFIX)/include/SDL2
    LIB_FLAGS = -L$(SDL2_PREFIX)/lib -L$(SDL2_IMAGE_PREFIX)/lib -L$(SDL2_TTF_PREFIX)/lib -L$(SDL2_MIXER_PREFIX)/lib
else
    # Linux - use system paths
    INCLUDE_FLAGS =
    LIB_FLAGS =
endif

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall -std=c99 $(INCLUDE_FLAGS)

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer $(LIB_FLAGS)

#OBJ_NAME specifies the name of our executable
OBJ_NAME = minesweeper

#This is the target that compiles our executable
all : $(OBJS)
		$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

#Clean
.PHONY: clean

clean:
	rm -f *.o *~ $(OBJ_NAME)
