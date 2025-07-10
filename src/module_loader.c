#include <windows.h>
#include <stdio.h>
#include "coordinator.h"
#include "module_interface.h"

// Load a module (DLL) and call its register_module function.
void load_module(const char* modulePath, Coordinator* coordinator) {
    HMODULE handle = LoadLibraryA(modulePath);
    if (!handle) {
        fprintf(stderr, "Error loading module %s\n", modulePath);
        return;
    }
    ModuleRegisterFunc register_module = (ModuleRegisterFunc)GetProcAddress(handle, "register_module");
    if (!register_module) {
        fprintf(stderr, "Error finding register_module in %s\n", modulePath);
        FreeLibrary(handle);
        return;
    }
    register_module(coordinator);
}
