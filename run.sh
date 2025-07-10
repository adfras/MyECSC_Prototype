#!/usr/bin/env sh
# Build and run the MyECSC prototype
set -e

# Determine SDL3 compile and link flags
if command -v sdl3-config >/dev/null 2>&1; then
    SDL_CFLAGS="$(sdl3-config --cflags)"
    SDL_LIBS="$(sdl3-config --libs)"
    USE_SDL=1
elif command -v pkg-config >/dev/null 2>&1 && pkg-config --exists sdl3; then
    SDL_CFLAGS="$(pkg-config --cflags sdl3)"
    SDL_LIBS="$(pkg-config --libs sdl3)"
    USE_SDL=1
else
    echo "SDL3 development libraries not found. Building in headless mode." >&2
    USE_SDL=0
    SDL_CFLAGS=""
    SDL_LIBS=""
fi

# Source files needed to build the demo
SRCS="src/main.c src/coordinator.c src/entity_manager.c src/ComponentManager.c src/physics_system.c src/debug_module.c src/module.c"
if [ "$USE_SDL" -eq 1 ]; then
    SRCS="$SRCS src/render3d_system.c"
    CFLAGS_EXTRA="-DUSE_SDL"
else
    CFLAGS_EXTRA=""
fi

# Build the executable
gcc -std=c11 $CFLAGS_EXTRA $SDL_CFLAGS -Iinclude $SRCS -o MyECSC_Prototype $SDL_LIBS

# Run the program
./MyECSC_Prototype
