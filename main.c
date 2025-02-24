#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "entity_manager.h" 
#include "ComponentArray.h"

#define MAX_ENTITIES 5000
#define NUM_COMPONENT_TYPES 1  // Adjust as needed

// -------------------------------
// Component Definitions
// -------------------------------


typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Quat;

typedef struct {
    Vec3 position;
    Quat rotation;
    Vec3 scale;
} Transform;

typedef uint32_t Entity;  // Already defined as uint32_t elsewhere if needed.

// Generate the Transform component array using the macro.
DEFINE_COMPONENT_ARRAY(Transform);

int main(void) {

    //Initialize the EntityManager
    EntityManager* manager = malloc(sizeof(EntityManager));
    if (!manager)
    {
        return 1; // Allocation failure
    }
    EntityManager_Init(manager);

    // Create an entity using the entity manager
    uint32_t entity = EntityManager_CreateEntity(manager);
    EntityManager_SetSignature(manager, entity, 0x7);
    printf("Entity Manager: Entity %u created with signature %u\n", entity, EntityManager_GetSignature(manager, entity));

    // Initialize the Transform component array.
    TransformComponentArray transformArray;
    TransformComponentArray_Init(&transformArray);

    // (Optional) If you're maintaining a global array of component arrays, you might register it:
    // componentArrays[0] = (IComponentArray*)&transformArray;

    // Define and insert a Transform component
    Transform t = { {1.0f, 2.0f, 3.0f}, // position
                    {0.0f, 0.0f, 0.0f, 1.0f}, // rotation
                    {1.0f, 1.0f, 1.0f} }; // scale
	TransformComponentArray_Insert(&transformArray, entity, t);

	// Retrieve and print the component's position
	Transform* retrieved = TransformComponentArray_GetData(&transformArray, entity);
	printf("Transform Component: Entity %u has position (%f, %f, %f)\n", entity, retrieved->position.x, retrieved->position.y, retrieved->position.z);

	//Simulate entity destruction:
	// Notify the component arrays that an entity has been destroyed.
	transformArray.base.EntityDestroyed((IComponentArray*)&transformArray, entity);
	// Destroy the entity using the entity manager
	EntityManager_DestroyEntity(manager, entity);
	printf("Entity Manager: Destroyed entity %u. New signature: %u\n", entity, EntityManager_GetSignature(manager, entity));

	free(manager);
    return 0;
	}

