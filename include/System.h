// system.h
#ifndef SYSTEM_H
#define SYSTEM_H

#include "entity_manager.h" // For Entity and Signature definitions.
#include <assert.h>
#include <stdint.h>

#define MAX_SYSTEM_ENTITIES MAX_ENTITIES

typedef struct {
    Entity entities[MAX_SYSTEM_ENTITIES];
    int count;
    Signature requiredSignature;  // Bitmask representing required components.
} ECS_System;

// Add an entity to the system (ensuring no duplicates).
static inline void ECS_System_AddEntity(ECS_System* sys, Entity entity) {
    for (int i = 0; i < sys->count; i++) {
        if (sys->entities[i] == entity) {
            return; // Already present.
        }
    }
    assert(sys->count < MAX_SYSTEM_ENTITIES && "System entity list is full.");
    sys->entities[sys->count++] = entity;
}

// Remove an entity from the system.
static inline void ECS_System_RemoveEntity(ECS_System* sys, Entity entity) {
    int i;
    for (i = 0; i < sys->count; i++) {
        if (sys->entities[i] == entity) {
            break;
        }
    }
    if (i < sys->count) {
        for (; i < sys->count - 1; i++) {
            sys->entities[i] = sys->entities[i + 1];
        }
        sys->count--;
    }
}

#endif // SYSTEM_H
