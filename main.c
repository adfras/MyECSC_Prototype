// main.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "entity_manager.h"
#include "ComponentManager.h"
#include "ComponentArray.h"
#include "Components.h"
#include "ComponentTypes.h"
#include "TransformComponent.h"
#include "physics_component.h"
#include "physics_system.h"
#include "system.h"
#include "system_manager.h"


#define MAX_ENTITIES 5000

int main(void) {
    // Initialize the Entity Manager.
    EntityManager* entityManager = malloc(sizeof(EntityManager));
    if (!entityManager) {
        return 1; // Allocation failure.
    }
    EntityManager_Init(entityManager);

    // Create the Component Manager.
    ComponentManager componentManager = { 0 };

    // Initialize and register the Transform component array.
    TransformComponentArray transformArray;
    TransformComponentArray_Init(&transformArray);
    ComponentManager_RegisterComponent(&componentManager, COMPONENT_TRANSFORM, (IComponentArray*)&transformArray);

    // Initialize and register the Physics component array.
    PhysicsComponentArray physicsArray;
    PhysicsComponentArray_Init(&physicsArray);
    ComponentManager_RegisterComponent(&componentManager, COMPONENT_PHYSICS, (IComponentArray*)&physicsArray);

    // Initialize the Physics System and pass the Component Manager to it.
    PhysicsSystem physicsSystem;
    PhysicsSystem_Init(&physicsSystem, &componentManager);

    // Create an entity.
    Entity entity = EntityManager_CreateEntity(entityManager);

    // Set the entity's signature to include both Transform and Physics components.
    Signature signature = (1 << COMPONENT_TRANSFORM) | (1 << COMPONENT_PHYSICS);
    EntityManager_SetSignature(entityManager, entity, signature);
    printf("Entity Manager: Entity %u created with signature %u\n", entity, EntityManager_GetSignature(entityManager, entity));

    // Insert a Transform component.
    Transform t = {
        {1.0f, 2.0f, 3.0f},             // position
        {0.0f, 0.0f, 0.0f, 1.0f},         // rotation
        {1.0f, 1.0f, 1.0f}                // scale
    };
    TransformComponentArray_Insert(&transformArray, entity, t);

    // Insert a Physics component.
    Physics p = {
        {0.0f, 0.0f, 0.0f},               // initial velocity
        {0.0f, -9.8f, 0.0f}               // force (gravity)
    };
    PhysicsComponentArray_Insert(&physicsArray, entity, p);

    // Manually add the entity to the physics system if its signature matches.
    if ((signature & physicsSystem.base.requiredSignature) == physicsSystem.base.requiredSignature) {
        physicsSystem.base.entities[physicsSystem.base.count++] = entity;
    }

    // Print the initial Transform position.
    Transform* tr = TransformComponentArray_GetData(&transformArray, entity);
    printf("Before update: Entity %u position: (%f, %f, %f)\n", entity, tr->position.x, tr->position.y, tr->position.z);

    // Update the Physics System with a delta time of 1.0 seconds.
    PhysicsSystem_Update(&physicsSystem, 1.0f);

    // Print the updated Transform position.
    tr = TransformComponentArray_GetData(&transformArray, entity);
    printf("After update: Entity %u position: (%f, %f, %f)\n", entity, tr->position.x, tr->position.y, tr->position.z);

    // Also print the updated Physics velocity.
    Physics* ph = PhysicsComponentArray_GetData(&physicsArray, entity);
    printf("After update: Entity %u velocity: (%f, %f, %f)\n", entity, ph->velocity.x, ph->velocity.y, ph->velocity.z);

    // Simulate entity destruction.
    ComponentManager_EntityDestroyed(&componentManager, entity);
    EntityManager_DestroyEntity(entityManager, entity);
    printf("Entity Manager: Destroyed entity %u. New signature: %u\n", entity, EntityManager_GetSignature(entityManager, entity));

    free(entityManager);
    return 0;
}
