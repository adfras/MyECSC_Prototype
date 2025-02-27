#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "coordinator.h"
#include "entity_manager.h"
#include "ComponentManager.h"
#include "system_manager.h"
#include "ComponentTypes.h"
#include "TransformComponent.h"
#include "physics_component.h"
#include "physics_system.h"
#include "Components.h"

int main(void) {
    // Allocate and initialize managers.
    EntityManager* entityManager = malloc(sizeof(EntityManager));
    if (!entityManager) return 1;
    EntityManager_Init(entityManager);

    ComponentManager* componentManager = malloc(sizeof(ComponentManager));
    if (!componentManager) return 1;
    // Initialize all component array pointers to NULL.
    for (int i = 0; i < MAX_COMPONENT_TYPES; i++) {
        componentManager->componentArrays[i] = NULL;
    }

    SystemManager* systemManager = malloc(sizeof(SystemManager));
    if (!systemManager) return 1;
    systemManager->count = 0;

    // Create and register the Transform component array.
    TransformComponentArray* transformArray = malloc(sizeof(TransformComponentArray));
    if (!transformArray) return 1;
    TransformComponentArray_Init(transformArray);
    ComponentManager_RegisterComponent(componentManager, COMPONENT_TRANSFORM, (IComponentArray*)transformArray);

    // Create and register the Physics component array.
    PhysicsComponentArray* physicsArray = malloc(sizeof(PhysicsComponentArray));
    if (!physicsArray) return 1;
    PhysicsComponentArray_Init(physicsArray);
    ComponentManager_RegisterComponent(componentManager, COMPONENT_PHYSICS, (IComponentArray*)physicsArray);

    // Create and register the Physics system.
    PhysicsSystem* physicsSystem = malloc(sizeof(PhysicsSystem));
    if (!physicsSystem) return 1;
    PhysicsSystem_Init(physicsSystem, componentManager);
    SystemManager_AddSystem(systemManager, (ECS_System*)physicsSystem);

    // Initialize the coordinator with our managers.
    Coordinator coordinator;
    Coordinator_Init(&coordinator, entityManager, componentManager, systemManager);

    // Create an entity via the coordinator.
    Entity entity = Coordinator_CreateEntity(&coordinator);

    // Add a Transform component (initial position (0,0,0)) via the coordinator.
    Transform t = {
        {0.0f, 0.0f, 0.0f},             // position
        {0.0f, 0.0f, 0.0f, 1.0f},         // rotation (identity quaternion)
        {1.0f, 1.0f, 1.0f}                // scale
    };
    Coordinator_AddTransform(&coordinator, entity, t);

    // Add a Physics component (velocity (1,0,0) and zero force) via the coordinator.
    Physics p = {
        {1.0f, 0.0f, 0.0f},              // initial velocity along x-axis
        {0.0f, 0.0f, 0.0f}               // no force applied
    };
    Coordinator_AddPhysics(&coordinator, entity, p);

    // Retrieve and print the Transform component before updating.
    Transform* transform = Coordinator_GetTransform(&coordinator, entity);
    printf("Before update: Entity %u Transform position: (%f, %f, %f)\n",
        entity,
        transform->position.x, transform->position.y, transform->position.z);

    // Update the Physics system (simulate one second elapsed).
    PhysicsSystem_Update(physicsSystem, 1.0f);

    // Retrieve and print the updated Transform and Physics components.
    transform = Coordinator_GetTransform(&coordinator, entity);
    Physics* physics = Coordinator_GetPhysics(&coordinator, entity);
    printf("After update: Entity %u Transform position: (%f, %f, %f)\n",
        entity,
        transform->position.x, transform->position.y, transform->position.z);
    printf("After update: Entity %u Physics velocity: (%f, %f, %f)\n",
        entity,
        physics->velocity.x, physics->velocity.y, physics->velocity.z);

    // Destroy the entity via the coordinator.
    Coordinator_DestroyEntity(&coordinator, entity);
    printf("Entity %u destroyed.\n", entity);

    // Clean up.
    free(physicsSystem);
    free(physicsArray);
    free(transformArray);
    free(systemManager);
    free(componentManager);
    free(entityManager);

    return 0;
}
