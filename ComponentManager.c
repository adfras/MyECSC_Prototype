//ComponentManager.c
#include "ComponentManager.h"
#include "ComponentArray.h"
#include "Components.h" 
#include "TransformComponent.h" 

#include <assert.h>

// Register a component array for a given type.
void ComponentManager_RegisterComponent(ComponentManager* mgr, ComponentType type, IComponentArray* array)
{
	assert(type < MAX_COMPONENT_TYPES && "Component type out of range.");
	mgr->componentArrays[type] = array;
}

// Notify all component arrays that an entity has been destroyed.
void ComponentManager_EntityDestroyed(ComponentManager* mgr, uint32_t entity)
{
	for (int i = 0; i < MAX_COMPONENT_TYPES; i++)
	{
		if (mgr->componentArrays[i]) 
			{
			// Call the EntityDestroyed function pointer on each registered component array
			mgr->componentArrays[i]->EntityDestroyed(mgr->componentArrays[i], entity);
			}
		{

		}
	}
}


