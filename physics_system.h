#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "entity_manager.h"       // For Entity and Signature.
#include "ComponentManager.h"     // For the ComponentManager.
#include "ComponentTypes.h"       // For COMPONENT_TRANSFORM and COMPONENT_PHYSICS.
#include "TransformComponent.h"   // Your existing transform component.
#include "physics_component.h"     // The physics component we just defined.
#include "system.h"               // Include the base ECS_System.

// Base System structure.
typedef struct {
    ECS_System base;              // Our renamed base system.
    ComponentManager* componentManager;  // To retrieve component arrays.
} PhysicsSystem;

// Initializes the physics system by setting its required signature and storing the ComponentManager.
void PhysicsSystem_Init(PhysicsSystem* psys, ComponentManager* cm);

// Updates the physics system by applying simple physics (Euler integration) to all entities.
void PhysicsSystem_Update(PhysicsSystem* psys, float dt);

#endif // PHYSICS_SYSTEM_H
