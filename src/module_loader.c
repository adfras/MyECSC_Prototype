#include <stdio.h>
#include "coordinator.h"
#include "module_interface.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

// Load a module and call its register_module function.
void load_module(const char* modulePath, Coordinator* coordinator) {
#ifdef _WIN32
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
    FreeLibrary(handle);
#else
    dlerror(); // Clear any existing error
    void* handle = dlopen(modulePath, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Error loading module %s: %s\n", modulePath, dlerror());
        return;
    }
    ModuleRegisterFunc register_module = (ModuleRegisterFunc)dlsym(handle, "register_module");
    const char* err = dlerror();
    if (err) {
        fprintf(stderr, "Error finding register_module in %s: %s\n", modulePath, err);
        dlclose(handle);
        return;
    }
    register_module(coordinator);
    dlclose(handle);
#endif
}
