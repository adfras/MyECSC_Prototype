#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "entity_manager.h"
#include "ComponentManager.h"
#include "ComponentTypes.h"
#include "TransformComponent.h"
#include "rigid_body_component.h"
#include "gravity_component.h"
#include "System.h"           

// Base System structure.
typedef struct {
    ECS_System base;  // Contains the entity list and the required signature.
    ComponentManager* componentManager;
} PhysicsSystem;

// Initializes the physics system by setting its required signature and storing the ComponentManager.
void PhysicsSystem_Init(PhysicsSystem* psys, ComponentManager* cm);

// Updates the physics system by applying simple physics (Euler integration) to all entities.
void PhysicsSystem_Update(PhysicsSystem* psys, float dt);

#endif // PHYSICS_SYSTEM_H
