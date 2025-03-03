#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "coordinator.h"
#include "entity_manager.h"
#include "ComponentManager.h"
#include "system_manager.h"
#include "ComponentTypes.h"
#include "TransformComponent.h"
#include "gravity_component.h"
#include "rigid_body_component.h"
#include "physics_system.h"
#include "Components.h"
#include "render3d_system.h"
#include "module_interface.h"
#include "debug_module.h"


// Global so render3d_system.c can use it
SDL_Color entityColors[MAX_ENTITIES];

int main(void) {
    // --- Initialize ECS Managers ---
    EntityManager* entityManager = malloc(sizeof(EntityManager));
    if (!entityManager) return 1;
    EntityManager_Init(entityManager);

    ComponentManager* componentManager = malloc(sizeof(ComponentManager));
    if (!componentManager) return 1;
    for (int i = 0; i < MAX_COMPONENT_TYPES; i++) {
        componentManager->componentArrays[i] = NULL;
    }

    SystemManager* systemManager = malloc(sizeof(SystemManager));
    if (!systemManager) return 1;
    systemManager->count = 0;

    // Register component arrays
    TransformComponentArray* transformArray = malloc(sizeof(TransformComponentArray));
    if (!transformArray) return 1;
    TransformComponentArray_Init(transformArray);
    ComponentManager_RegisterComponent(componentManager, COMPONENT_TRANSFORM, (IComponentArray*)transformArray);

    GravityComponentArray* gravityArray = malloc(sizeof(GravityComponentArray));
    if (!gravityArray) return 1;
    GravityComponentArray_Init(gravityArray);
    ComponentManager_RegisterComponent(componentManager, COMPONENT_GRAVITY, (IComponentArray*)gravityArray);

    RigidBodyComponentArray* rigidBodyArray = malloc(sizeof(RigidBodyComponentArray));
    if (!rigidBodyArray) return 1;
    RigidBodyComponentArray_Init(rigidBodyArray);
    ComponentManager_RegisterComponent(componentManager, COMPONENT_RIGID_BODY, (IComponentArray*)rigidBodyArray);

    // --- Register Systems ---
    PhysicsSystem* physicsSystem = malloc(sizeof(PhysicsSystem));
    if (!physicsSystem) return 1;
    PhysicsSystem_Init(physicsSystem, componentManager);
    SystemManager_AddSystem(systemManager, (ECS_System*)physicsSystem);

    Render3DSystem render3dSystem;
    Render3DSystem_Init(&render3dSystem, componentManager);
    SystemManager_AddSystem(systemManager, (ECS_System*)&render3dSystem);

    // Initialize the coordinator
    Coordinator coordinator;
    Coordinator_Init(&coordinator, entityManager, componentManager, systemManager);

    // Register Modules (such as debug module)
    register_module(&coordinator);

    // Set up randomization
    srand((unsigned)time(NULL));
    int numEntities = 200;

    // Create entities with wide random positions, random rotations, negative Y gravity
    for (int i = 0; i < numEntities; i++) {
        Entity entity = Coordinator_CreateEntity(&coordinator);

        // Spread them out more in X and Y
        float randPosX = ((float)rand() / RAND_MAX) * 300.0f - 150.0f;
        float randPosY = ((float)rand() / RAND_MAX) * 300.0f - 150.0f;
        float randPosZ = ((float)rand() / RAND_MAX) * 100.0f + 50.0f; 

        // Random 3-axis rotation
        float randRotX = ((float)rand() / RAND_MAX) * 6.283185f;  // 0..2?
        float randRotY = ((float)rand() / RAND_MAX) * 6.283185f;
        float randRotZ = ((float)rand() / RAND_MAX) * 6.283185f;

        
        float scale = 5.0f;

        // Gravity downward along Y
        Gravity g = { { 0.0f, -9.8f, 0.0f } };

        // No initial velocity
        RigidBody rb = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

        // Transform has position, rotation, scale
        Transform t = {
            { randPosX, randPosY, randPosZ },
            { randRotX, randRotY, randRotZ },
            { scale, scale, scale }
        };

        // Assign random color
        entityColors[entity].r = rand() % 256;
        entityColors[entity].g = rand() % 256;
        entityColors[entity].b = rand() % 256;
        entityColors[entity].a = 255;

        Coordinator_AddGravity(&coordinator, entity, g);
        Coordinator_AddRigidBody(&coordinator, entity, rb);
        Coordinator_AddTransform(&coordinator, entity, t);
    }

    // ---- Initialize SDL3 ----
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Falling Blocks Demo", 1280, 720, 0);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, "opengl");
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop
    int quit = 0;
    SDL_Event event;
    float dt = 0.016f; // ~60 FPS
    int iterations = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = 1;
            }
        }

        // Update physics
        PhysicsSystem_Update(physicsSystem, dt);


        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render 3D cubes
        Render3DSystem_Update(&render3dSystem, dt, renderer, 1280, 720);

        // Demonstrate the Debug module purpose.
        if (gDebugSystem) {
            DebugSystem_Run(gDebugSystem, dt);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        iterations++;
        if (iterations > 1000)
            quit = 1;
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(physicsSystem);
    free(rigidBodyArray);
    free(gravityArray);
    free(transformArray);
    free(systemManager);
    free(componentManager);
    free(entityManager);

    return 0;
}
