#include "coordinator.h"
#include <assert.h>

void Coordinator_Init(Coordinator* coordinator,
    EntityManager* entityManager,
    ComponentManager* componentManager,
    SystemManager* systemManager) {
    coordinator->entityManager = entityManager;
    coordinator->componentManager = componentManager;
    coordinator->systemManager = systemManager;
}

Entity Coordinator_CreateEntity(Coordinator* coordinator) {
    return EntityManager_CreateEntity(coordinator->entityManager);
}

void Coordinator_DestroyEntity(Coordinator* coordinator, Entity entity) {
    EntityManager_DestroyEntity(coordinator->entityManager, entity);
    ComponentManager_EntityDestroyed(coordinator->componentManager, entity);
    SystemManager_EntityDestroyed(coordinator->systemManager, entity);
}

// Add a Transform component by directly inserting it into the transform array.
void Coordinator_AddTransform(Coordinator* coordinator, Entity entity, Transform component) {
    // Retrieve the TransformComponentArray from the ComponentManager.
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_TRANSFORM];
    assert(base && "Transform component array not registered.");
    TransformComponentArray* transformArray = (TransformComponentArray*)base;
    TransformComponentArray_Insert(transformArray, entity, component);

    // Update the entity's signature.
    Signature signature = EntityManager_GetSignature(coordinator->entityManager, entity);
    signature |= (1 << COMPONENT_TRANSFORM);
    EntityManager_SetSignature(coordinator->entityManager, entity, signature);

    // Notify systems about the updated signature.
    SystemManager_EntitySignatureChanged(coordinator->systemManager, entity, signature);
}

Transform* Coordinator_GetTransform(Coordinator* coordinator, Entity entity) {
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_TRANSFORM];
    assert(base && "Transform component array not registered.");
    TransformComponentArray* transformArray = (TransformComponentArray*)base;
    return TransformComponentArray_GetData(transformArray, entity);
}

// Add a Physics component by directly inserting it into the physics array.
void Coordinator_AddPhysics(Coordinator* coordinator, Entity entity, Physics component) {
    // Retrieve the PhysicsComponentArray from the ComponentManager.
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_PHYSICS];
    assert(base && "Physics component array not registered.");
    PhysicsComponentArray* physicsArray = (PhysicsComponentArray*)base;
    PhysicsComponentArray_Insert(physicsArray, entity, component);

    // Update the entity's signature.
    Signature signature = EntityManager_GetSignature(coordinator->entityManager, entity);
    signature |= (1 << COMPONENT_PHYSICS);
    EntityManager_SetSignature(coordinator->entityManager, entity, signature);

    // Notify systems about the updated signature.
    SystemManager_EntitySignatureChanged(coordinator->systemManager, entity, signature);
}

Physics* Coordinator_GetPhysics(Coordinator* coordinator, Entity entity) {
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_PHYSICS];
    assert(base && "Physics component array not registered.");
    PhysicsComponentArray* physicsArray = (PhysicsComponentArray*)base;
    return PhysicsComponentArray_GetData(physicsArray, entity);
}
