#ifndef DEBUG_MODULE_H
#define DEBUG_MODULE_H

#include "coordinator.h"
#include "System.h"

// Define the DebugSystem structure.
typedef struct {
    ECS_System base;
    // Additional debug-specific data can be added here.
} DebugSystem;

// Global pointer to the DebugSystem instance.
extern DebugSystem* gDebugSystem;

// Function prototypes.
void register_module(Coordinator* coordinator);
void DebugSystem_Run(DebugSystem* ds, float dt);

#endif // DEBUG_MODULE_H
