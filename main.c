#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "entity_manager.h" 

#define MAX_ENTITIES 5000
#define NUM_COMPONENT_TYPES 1  // Adjust as needed

// Forward declaration of IComponentArray.
typedef struct IComponentArray IComponentArray;

// Base “interface” for component arrays.
struct IComponentArray {
    void (*EntityDestroyed)(IComponentArray* self, uint32_t entity);
};

// Global array holding pointers to all component arrays.
IComponentArray* componentArrays[NUM_COMPONENT_TYPES];


// -------------------------------
// Component Definitions
// -------------------------------

// Transform component definitions.
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


// -------------------------------
// TransformComponentArray Definition
// -------------------------------

typedef struct {
    IComponentArray base;                   // Base “interface”
    Transform components[MAX_ENTITIES];     // Dense array of Transform components.
    int entityToIndexMap[MAX_ENTITIES];       // Maps an entity to its index in the array (-1 if not present)
    Entity indexToEntityMap[MAX_ENTITIES];    // Reverse mapping: index to entity.
    size_t size;                            // Number of valid components.
} TransformComponentArray;


// -------------------------------
// Function Prototypes
// -------------------------------

void TransformComponentArray_Init(TransformComponentArray* arr);
void TransformComponentArray_Insert(TransformComponentArray* arr, Entity entity, Transform component);
void TransformComponentArray_RemoveData(TransformComponentArray* arr, Entity entity);
Transform* TransformComponentArray_GetData(TransformComponentArray* arr, Entity entity);
void TransformComponentArray_EntityDestroyed(IComponentArray* base, Entity entity);
void NotifyEntityDestroyed(Entity entity);


// -------------------------------
// Function Implementations
// -------------------------------

void TransformComponentArray_Init(TransformComponentArray* arr) {
    // Set the function pointer for the “virtual” callback.
    arr->base.EntityDestroyed = TransformComponentArray_EntityDestroyed;
    arr->size = 0;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        arr->entityToIndexMap[i] = -1; // -1 indicates no component for this entity.
    }
}

void TransformComponentArray_Insert(TransformComponentArray* arr, Entity entity, Transform component) {
    assert(arr->entityToIndexMap[entity] == -1 && "Component already exists for entity.");
    size_t newIndex = arr->size;
    arr->entityToIndexMap[entity] = newIndex;
    arr->indexToEntityMap[newIndex] = entity;
    arr->components[newIndex] = component;
    arr->size++;
}

void TransformComponentArray_RemoveData(TransformComponentArray* arr, Entity entity) {
    assert(arr->entityToIndexMap[entity] != -1 && "Component does not exist for entity.");
    int indexOfRemovedEntity = arr->entityToIndexMap[entity];
    size_t indexOfLastElement = arr->size - 1;

    // Move the last component into the removed component's slot.
    arr->components[indexOfRemovedEntity] = arr->components[indexOfLastElement];

    // Update the mapping for the moved component.
    Entity entityOfLastElement = arr->indexToEntityMap[indexOfLastElement];
    arr->entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    arr->indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    // Mark the removed entity as having no component.
    arr->entityToIndexMap[entity] = -1;
    arr->size--;
}

Transform* TransformComponentArray_GetData(TransformComponentArray* arr, Entity entity) {
    assert(arr->entityToIndexMap[entity] != -1 && "Component does not exist for entity.");
    int index = arr->entityToIndexMap[entity];
    return &arr->components[index];
}

// This function is called via the base interface when an entity is destroyed.
void TransformComponentArray_EntityDestroyed(IComponentArray* base, Entity entity) {
    TransformComponentArray* arr = (TransformComponentArray*)base;
    if (arr->entityToIndexMap[entity] != -1) {
        TransformComponentArray_RemoveData(arr, entity);
    }
}

// Notify all component arrays that an entity has been destroyed.
void NotifyEntityDestroyed(Entity entity) {
    for (int i = 0; i < NUM_COMPONENT_TYPES; i++) {
        if (componentArrays[i] != NULL) {
            componentArrays[i]->EntityDestroyed(componentArrays[i], entity);
        }
    }
}


// -------------------------------
// Example Main (for demonstration)
// -------------------------------

int main(void) {

    // Allocate and initialize the entity manager.
    EntityManager* manager = malloc(sizeof(EntityManager));
    if (manager == NULL) {
        return 1; // Allocation failure.
    }
    EntityManager_Init(manager);

    // Create an entity using the entity manager.
    Entity e = EntityManager_CreateEntity(manager);
    EntityManager_SetSignature(manager, e, 0x7);
    printf("Entity Manager: Entity %u created with signature %u\n", e, EntityManager_GetSignature(manager, e));

    // Initialize the Transform component array and register it.
    TransformComponentArray transformArray;
    TransformComponentArray_Init(&transformArray);
    componentArrays[0] = (IComponentArray*)&transformArray;

    // Example: Insert a Transform component for an entity.
    Entity entity = 1;  // Example entity ID.
    Transform t = { {1.0f, 2.0f, 3.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f} };
    TransformComponentArray_Insert(&transformArray, entity, t);

    // Retrieve and print the component data.
    Transform* retrieved = TransformComponentArray_GetData(&transformArray, entity);
    printf("Entity %u Transform position: (%f, %f, %f)\n",
        entity, retrieved->position.x, retrieved->position.y, retrieved->position.z);

    // Simulate entity destruction:
   // 1. Notify all component arrays that the entity is destroyed.
    NotifyEntityDestroyed(e);
    // 2. Destroy the entity via the entity manager.
    EntityManager_DestroyEntity(manager, e);
    printf("Entity Manager: Entity %u destroyed. New signature: %u\n", e, EntityManager_GetSignature(manager, e));

    free(manager);
    return 0;
}
