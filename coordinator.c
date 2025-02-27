#include "coordinator.h"
#include <assert.h>

// Initialize the Coordinator with pointers to the managers.
void Coordinator_Init(Coordinator* coordinator,
    EntityManager* entityManager,
    ComponentManager* componentManager,
    SystemManager* systemManager)
{
    coordinator->entityManager = entityManager;
    coordinator->componentManager = componentManager;
    coordinator->systemManager = systemManager;
}

// Create a new entity using the Entity Manager.
Entity Coordinator_CreateEntity(Coordinator* coordinator) {
    return EntityManager_CreateEntity(coordinator->entityManager);
}

// Destroy an entity and notify all managers.
void Coordinator_DestroyEntity(Coordinator* coordinator, Entity entity) {
    EntityManager_DestroyEntity(coordinator->entityManager, entity);
    ComponentManager_EntityDestroyed(coordinator->componentManager, entity);
    SystemManager_EntityDestroyed(coordinator->systemManager, entity);
}

// --- Transform Component Functions ---

// Add a Transform component to an entity.
void Coordinator_AddTransform(Coordinator* coordinator, Entity entity, Transform component) {
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_TRANSFORM];
    assert(base && "Transform component array not registered.");
    TransformComponentArray* transformArray = (TransformComponentArray*)base;
    TransformComponentArray_Insert(transformArray, entity, component);

    // Update the entity's signature.
    Signature signature = EntityManager_GetSignature(coordinator->entityManager, entity);
    signature |= (1 << COMPONENT_TRANSFORM);
    EntityManager_SetSignature(coordinator->entityManager, entity, signature);

    // Notify systems about the signature change.
    SystemManager_EntitySignatureChanged(coordinator->systemManager, entity, signature);
}

// Retrieve a pointer to the Transform component of an entity.
Transform* Coordinator_GetTransform(Coordinator* coordinator, Entity entity) {
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_TRANSFORM];
    assert(base && "Transform component array not registered.");
    TransformComponentArray* transformArray = (TransformComponentArray*)base;
    return TransformComponentArray_GetData(transformArray, entity);
}

// --- Gravity Component Functions ---

// Add a Gravity component to an entity.
void Coordinator_AddGravity(Coordinator* coordinator, Entity entity, Gravity component) {
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_GRAVITY];
    assert(base && "Gravity component array not registered.");
    GravityComponentArray* gravityArray = (GravityComponentArray*)base;
    GravityComponentArray_Insert(gravityArray, entity, component);

    // Update the entity's signature.
    Signature signature = EntityManager_GetSignature(coordinator->entityManager, entity);
    signature |= (1 << COMPONENT_GRAVITY);
    EntityManager_SetSignature(coordinator->entityManager, entity, signature);

    // Notify systems about the signature change.
    SystemManager_EntitySignatureChanged(coordinator->systemManager, entity, signature);
}

// Retrieve a pointer to the Gravity component of an entity.
Gravity* Coordinator_GetGravity(Coordinator* coordinator, Entity entity) {
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_GRAVITY];
    assert(base && "Gravity component array not registered.");
    GravityComponentArray* gravityArray = (GravityComponentArray*)base;
    return GravityComponentArray_GetData(gravityArray, entity);
}

// --- RigidBody Component Functions ---

// Add a RigidBody component to an entity.
void Coordinator_AddRigidBody(Coordinator* coordinator, Entity entity, RigidBody component) {
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_RIGID_BODY];
    assert(base && "RigidBody component array not registered.");
    RigidBodyComponentArray* rbArray = (RigidBodyComponentArray*)base;
    RigidBodyComponentArray_Insert(rbArray, entity, component);

    // Update the entity's signature.
    Signature signature = EntityManager_GetSignature(coordinator->entityManager, entity);
    signature |= (1 << COMPONENT_RIGID_BODY);
    EntityManager_SetSignature(coordinator->entityManager, entity, signature);

    // Notify systems about the signature change.
    SystemManager_EntitySignatureChanged(coordinator->systemManager, entity, signature);
}

// Retrieve a pointer to the RigidBody component of an entity.
RigidBody* Coordinator_GetRigidBody(Coordinator* coordinator, Entity entity) {
    IComponentArray* base = coordinator->componentManager->componentArrays[COMPONENT_RIGID_BODY];
    assert(base && "RigidBody component array not registered.");
    RigidBodyComponentArray* rbArray = (RigidBodyComponentArray*)base;
    return RigidBodyComponentArray_GetData(rbArray, entity);
}
