#pragma once

#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_ENTITIES 5000

// Base "interface" for component arrays.
typedef struct IComponentArray {
    void (*EntityDestroyed)(struct IComponentArray* self, uint32_t entity);
} IComponentArray;

// Macro to define a component array for any component type.
#define DEFINE_COMPONENT_ARRAY(ComponentType)                                                   \
    typedef struct {                                                                            \
        IComponentArray base;              /* Base interface */                                 \
        ComponentType components[MAX_ENTITIES]; /* Dense array of components */                   \
        int entityToIndexMap[MAX_ENTITIES];  /* Maps an entity to its index (-1 if absent) */      \
        uint32_t indexToEntityMap[MAX_ENTITIES]; /* Reverse mapping: index to entity */            \
        size_t size;                       /* Number of valid components */                     \
    } ComponentType##ComponentArray;                                                            \
                                                                                                \
    /* Forward declaration of the EntityDestroyed function */                                   \
    static inline void ComponentType##ComponentArray_EntityDestroyed(IComponentArray* base, uint32_t entity); \
                                                                                                \
    /* Initialization function for a component array */                                         \
    static inline void ComponentType##ComponentArray_Init(ComponentType##ComponentArray* arr) {   \
        arr->base.EntityDestroyed = (void (*)(IComponentArray*, uint32_t))                       \
            ComponentType##ComponentArray_EntityDestroyed;                                      \
        arr->size = 0;                                                                          \
        for (int i = 0; i < MAX_ENTITIES; i++) {                                                \
            arr->entityToIndexMap[i] = -1;                                                      \
        }                                                                                       \
    }                                                                                           \
                                                                                                \
    /* Insert a component into the array */                                                     \
    static inline void ComponentType##ComponentArray_Insert(ComponentType##ComponentArray* arr,    \
                                                             uint32_t entity,                 \
                                                             ComponentType component) {       \
        assert(arr->entityToIndexMap[entity] == -1 && "Component already exists for entity.");   \
        size_t newIndex = arr->size;                                                            \
        arr->entityToIndexMap[entity] = (int)newIndex; /* cast to int to avoid warning */        \
        arr->indexToEntityMap[newIndex] = entity;                                               \
        arr->components[newIndex] = component;                                                  \
        arr->size++;                                                                            \
    }                                                                                           \
                                                                                                \
    /* Remove a component from the array */                                                     \
    static inline void ComponentType##ComponentArray_RemoveData(ComponentType##ComponentArray* arr, \
                                                                  uint32_t entity) {             \
        assert(arr->entityToIndexMap[entity] != -1 && "Component does not exist for entity.");   \
        int indexOfRemovedEntity = arr->entityToIndexMap[entity];                               \
        size_t indexOfLastElement = arr->size - 1;                                                \
        /* Move the last component into the place of the removed component. */                  \
        arr->components[indexOfRemovedEntity] = arr->components[indexOfLastElement];              \
        /* Update the mapping for the entity that moved. */                                     \
        uint32_t entityOfLastElement = arr->indexToEntityMap[indexOfLastElement];                 \
        arr->entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;                        \
        arr->indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;                        \
        /* Mark the removed entity as having no component. */                                   \
        arr->entityToIndexMap[entity] = -1;                                                       \
        arr->size--;                                                                            \
    }                                                                                           \
                                                                                                \
    /* Retrieve a pointer to a component from the array */                                      \
    static inline ComponentType* ComponentType##ComponentArray_GetData(ComponentType##ComponentArray* arr, \
                                                                         uint32_t entity) {          \
        assert(arr->entityToIndexMap[entity] != -1 && "Component does not exist for entity.");   \
        int index = arr->entityToIndexMap[entity];                                               \
        return &arr->components[index];                                                          \
    }                                                                                           \
                                                                                                \
    /* Callback function for when an entity is destroyed */                                     \
    static inline void ComponentType##ComponentArray_EntityDestroyed(IComponentArray* base,      \
                                                                      uint32_t entity) {         \
        ComponentType##ComponentArray* arr = (ComponentType##ComponentArray*)base;              \
        if (arr->entityToIndexMap[entity] != -1) {                                                \
            ComponentType##ComponentArray_RemoveData(arr, entity);                              \
        }                                                                                       \
    }

#endif // COMPONENT_ARRAY_H
