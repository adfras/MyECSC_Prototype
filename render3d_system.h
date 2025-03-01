#ifndef RENDER3D_SYSTEM_H
#define RENDER3D_SYSTEM_H

#include "System.h"
#include "entity_manager.h"
#include "ComponentManager.h"
#include "ComponentTypes.h"
#include "SDL3/SDL.h"
#include <math.h>

// Render3DSystem structure that holds a base ECS_System and a pointer to the ComponentManager.
typedef struct {
    ECS_System base;            // Contains the list of entities and required signature.
    ComponentManager* componentManager;
    // You could add more fields here for projection settings, etc.
} Render3DSystem;

// Initialize the Render3DSystem with the ComponentManager.
void Render3DSystem_Init(Render3DSystem* r3dSys, ComponentManager* cm);

// Update the Render3DSystem: for each entity, render a 3D wireframe cube.
// The renderer, dt, and screen parameters are passed in.
void Render3DSystem_Update(Render3DSystem* r3dSys, float dt, SDL_Renderer* renderer, int screenWidth, int screenHeight);

#endif // RENDER3D_SYSTEM_H
