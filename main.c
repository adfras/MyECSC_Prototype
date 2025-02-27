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

int main(void) {
    // Allocate and initialize managers.
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

    // Register component arrays.
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

    // Create and register the Physics system.
    PhysicsSystem* physicsSystem = malloc(sizeof(PhysicsSystem));
    if (!physicsSystem) return 1;
    PhysicsSystem_Init(physicsSystem, componentManager);
    SystemManager_AddSystem(systemManager, (ECS_System*)physicsSystem);

    // Initialize the coordinator.
    Coordinator coordinator;
    Coordinator_Init(&coordinator, entityManager, componentManager, systemManager);

    // Set up randomization.
    srand((unsigned)time(NULL));
    int numEntities = 10000;

    // Create 10,000 entities with randomized Gravity, RigidBody, and Transform.
    for (int i = 0; i < numEntities; i++) {
        Entity entity = Coordinator_CreateEntity(&coordinator);

        // Random values:
        // Position in range [-100, 100].
        float randPosX = ((float)rand() / RAND_MAX) * 200.0f - 100.0f;
        float randPosY = ((float)rand() / RAND_MAX) * 200.0f - 100.0f;
        float randPosZ = ((float)rand() / RAND_MAX) * 200.0f - 100.0f;
        // Rotation in range [0, 3].
        float randRot = ((float)rand() / RAND_MAX) * 3.0f;
        // Uniform scale in range [3, 5].
        float scale = ((float)rand() / RAND_MAX) * 2.0f + 3.0f;
        // Gravity force in range [-10, -1] on Y.
        float randGravity = ((float)rand() / RAND_MAX) * (-9.0f) - 1.0f;

        Gravity g = { { 0.0f, randGravity, 0.0f } };
        RigidBody rb = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        Transform t = { { randPosX, randPosY, randPosZ },
                        { randRot, randRot, randRot },
                        { scale, scale, scale } };

        Coordinator_AddGravity(&coordinator, entity, g);
        Coordinator_AddRigidBody(&coordinator, entity, rb);
        Coordinator_AddTransform(&coordinator, entity, t);
    }

    // Main loop simulation.
    // For demonstration, we run a fixed number of updates.
 // Main loop simulation.
    int quit = 0;
    float dt = 0.016f; // Assumed delta time (~16ms per frame)
    int iterations = 0;
    while (!quit) {
        // Optionally, measure dt with a high-resolution timer here.

        // Update the physics system.
        PhysicsSystem_Update(physicsSystem, dt);

        // --- Debug Printing ---
        // Print the positions of the first few entities to verify they're updating.
        for (int i = 0; i < 5; i++) {
            // Retrieve the Transform component for entity i.
            Transform* t = Coordinator_GetTransform(&coordinator, i);
            if (t) {  // Ensure the component exists.
                printf("Frame %d: Entity %d position: (%f, %f, %f)\n",
                    iterations, i, t->position.x, t->position.y, t->position.z);
            }
        }
        printf("\n");

        // For demonstration, exit after 600 iterations.
        iterations++;
        if (iterations > 600) {
            quit = 1;
        }
    }

    // Cleanup.
    free(physicsSystem);
    free(rigidBodyArray);
    free(gravityArray);
    free(transformArray);
    free(systemManager);
    free(componentManager);
    free(entityManager);

    return 0;
}
