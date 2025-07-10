#include <stdio.h>
#include <stdlib.h>
#include "debug_module.h"
#include "module_interface.h"

// Global pointer for the DebugSystem.
DebugSystem* gDebugSystem = NULL;

// Internal function that demonstrates the debug functionality.
static void DebugSystem_Run_Impl(DebugSystem* ds, float dt) {
    // For demonstration, print out the pointer and current entity count.
    printf("DebugSystem_Run: ds=%p, count=%d, dt=%.3f\n", (void*)ds, ds->base.count, dt);
}

// Initialize the DebugSystem.
static void DebugSystem_Init(DebugSystem* ds) {
    ds->base.count = 0;
    // You can initialize additional fields here if needed.
}

// Exported function to register the debug module.
void register_module(Coordinator* coordinator) {
    DebugSystem* debugSys = malloc(sizeof(DebugSystem));
    if (!debugSys) {
        fprintf(stderr, "Failed to allocate DebugSystem\n");
        return;
    }
    DebugSystem_Init(debugSys);
    gDebugSystem = debugSys;
    SystemManager_AddSystem(coordinator->systemManager, (ECS_System*)debugSys);
    printf("Debug module registered with ECS.\n");
}

// Function to run the debug module for demonstration.
void DebugSystem_Run(DebugSystem* ds, float dt) {
    DebugSystem_Run_Impl(ds, dt);
}
