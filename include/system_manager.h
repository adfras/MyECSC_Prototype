#pragma once

// system_manager.h
#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "System.h"
#include "entity_manager.h"
#include "ComponentTypes.h"
#include <assert.h>

#define MAX_SYSTEMS 32

typedef struct {
    ECS_System* systems[MAX_SYSTEMS]; // Fixed struct name
    int count;
} SystemManager;

// Register a system with the system manager
static inline void SystemManager_AddSystem(SystemManager* mgr, ECS_System* sys) {
    assert(mgr->count < MAX_SYSTEMS && "System Manager is full");
    mgr->systems[mgr->count++] = sys;
}

// Called when an entity is destroyed
static inline void SystemManager_EntityDestroyed(SystemManager* mgr, Entity entity) {
    for (int i = 0; i < mgr->count; i++) {
        ECS_System_RemoveEntity(mgr->systems[i], entity); // Fixed function call
    }
}

// Called when the signature of an entity changes
static inline void SystemManager_EntitySignatureChanged(SystemManager* mgr, Entity entity, Signature entitySignature) {
    for (int i = 0; i < mgr->count; i++) {
        ECS_System* sys = mgr->systems[i];

        // Check if entity has all the required components
        if ((entitySignature & sys->requiredSignature) == sys->requiredSignature) {
            ECS_System_AddEntity(sys, entity); // Fixed function call
        }
        else {
            ECS_System_RemoveEntity(sys, entity); // Fixed function call
        }
    }
}

#endif // !SYSTEM_MANAGER_H
