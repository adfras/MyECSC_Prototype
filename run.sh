#!/usr/bin/env sh
# Build and run the MyECSC prototype
set -e

# Determine SDL3 compile and link flags
if command -v sdl3-config >/dev/null 2>&1; then
    SDL_CFLAGS="$(sdl3-config --cflags)"
    SDL_LIBS="$(sdl3-config --libs)"
elif command -v pkg-config >/dev/null 2>&1 && pkg-config --exists sdl3; then
    SDL_CFLAGS="$(pkg-config --cflags sdl3)"
    SDL_LIBS="$(pkg-config --libs sdl3)"
else
    echo "SDL3 development libraries not found. Please install SDL3." >&2
    exit 1
fi

# Source files needed to build the demo
SRCS="main.c coordinator.c entity_manager.c ComponentManager.c physics_system.c render3d_system.c debug_module.c module.c"

# Build the executable
gcc -std=c11 $SDL_CFLAGS -I. $SRCS -o MyECSC_Prototype $SDL_LIBS

# Run the program
./MyECSC_Prototype
