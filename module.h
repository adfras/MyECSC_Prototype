#ifndef MODULE_H
#define MODULE_H

// A generic Module interface.
typedef struct Module {
    const char* name;
    void (*init)(void);       // Called once when the module is registered.
    void (*update)(float dt); // Called each frame (or on a schedule).
    // Additional functions (e.g., shutdown) can be added here.
} Module;

#define MAX_MODULES 32

extern Module* moduleRegistry[MAX_MODULES];
extern int moduleCount;

// Function to register a module.
void registerModule(Module* mod);

#endif // MODULE_H
