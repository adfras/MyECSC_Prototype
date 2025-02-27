#pragma once

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <stdint.h>
#include "ComponentArray.h"      // Contains the base IComponentArray struct.
#include "ComponentTypes.h"      // Contains the ComponentType enum.
#include "Components.h"          // Now includes definitions for Entity and Transform.

// Define the maximum number of entities
#define MAX_COMPONENT_TYPES 32

// The ComponentManager is a collection of component arrays.
typedef struct ComponentManager {
	IComponentArray* componentArrays[MAX_COMPONENT_TYPES];
} ComponentManager;

// Register a component array for a given type.
void ComponentManager_RegisterComponent(ComponentManager* mgr, ComponentType type, IComponentArray* componentArray);

// Notify all component arrays that an entity has been destroyed.
void ComponentManager_EntityDestroyed(ComponentManager* mgr, uint32_t entity);


#endif
