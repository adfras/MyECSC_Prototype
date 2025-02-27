#ifndef COORDINATOR_H
#define COORDINATOR_H

#include "entity_manager.h"
#include "ComponentManager.h"
#include "system_manager.h"
#include "ComponentTypes.h"
#include "Components.h"
#include "TransformComponent.h"
#include "rigid_body_component.h"
#include "gravity_component.h"
#include "physics_component.h"

// The Coordinator bundles all the managers.
typedef struct {
    EntityManager* entityManager;
    ComponentManager* componentManager;
    SystemManager* systemManager;
} Coordinator;

// Initialization.
void Coordinator_Init(Coordinator* coordinator,
    EntityManager* entityManager,
    ComponentManager* componentManager,
    SystemManager* systemManager);

// Entity management.
Entity Coordinator_CreateEntity(Coordinator* coordinator);
void Coordinator_DestroyEntity(Coordinator* coordinator, Entity entity);

// Transform component management.
void Coordinator_AddTransform(Coordinator* coordinator, Entity entity, Transform component);
Transform* Coordinator_GetTransform(Coordinator* coordinator, Entity entity);

// Physics component management.
void Coordinator_AddPhysics(Coordinator* coordinator, Entity entity, Physics component);
Physics* Coordinator_GetPhysics(Coordinator* coordinator, Entity entity);

// Gravity component management.
void Coordinator_AddGravity(Coordinator* coordinator, Entity entity, Gravity component);
Gravity* Coordinator_GetGravity(Coordinator* coordinator, Entity entity);

// RigidBody component management.
void Coordinator_AddRigidBody(Coordinator* coordinator, Entity entity, RigidBody component);
RigidBody* Coordinator_GetRigidBody(Coordinator* coordinator, Entity entity);


#endif // COORDINATOR_H
