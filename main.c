#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "entity_manager.h" 
#include "ComponentArray.h"
#include "ComponentManager.h"
#include "Components.h"
#include "TransformComponent.h" 

#define MAX_ENTITIES 5000
#define NUM_COMPONENT_TYPES 1  // Adjust as needed


int main(void) {

    //Initialize the EntityManager
    EntityManager* entityManager = malloc(sizeof(EntityManager));
    if (!entityManager)
    {
        return 1; // Allocation failure
    }
    EntityManager_Init(entityManager);

    // Create an entity using the entity manager
    uint32_t entity = EntityManager_CreateEntity(entityManager);
    EntityManager_SetSignature(entityManager, entity, 0x7);
    printf("Entity Manager: Entity %u created with signature %u\n", entity, EntityManager_GetSignature(entityManager, entity));

    // Create and initialize the component manager.
    ComponentManager componentManager = { 0 };

    // Initialize the Transform component array and transform component array with the manager
    TransformComponentArray transformArray;
    TransformComponentArray_Init(&transformArray);
    ComponentManager_RegisterComponent(&componentManager, COMPONENT_TRANSFORM, (IComponentArray*)&transformArray);

    // Define and insert a Transform component
    Transform t = { {1.0f, 2.0f, 3.0f}, // position
                    {0.0f, 0.0f, 0.0f, 1.0f}, // rotation
                    {1.0f, 1.0f, 1.0f} }; // scale
	TransformComponentArray_Insert(&transformArray, entity, t);

	// Retrieve and print the component's position
	Transform* retrieved = TransformComponentArray_GetData(&transformArray, entity);
	printf("Transform Component: Entity %u has position (%f, %f, %f)\n", entity, retrieved->position.x, retrieved->position.y, retrieved->position.z);

	//Simulate entity destruction:
	transformArray.base.EntityDestroyed((IComponentArray*)&transformArray, entity);
	EntityManager_DestroyEntity(entityManager, entity);
	printf("Entity Manager: Destroyed entity %u. New signature: %u\n", entity, EntityManager_GetSignature(entityManager, entity));

    // Notify all component arrays via the ComponentManager
    ComponentManager_EntityDestroyed(&componentManager, entity);

	free(entityManager);
    return 0;
	}

