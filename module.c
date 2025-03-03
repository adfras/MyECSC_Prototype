#include "module.h"
#include <stdio.h>

Module* moduleRegistry[MAX_MODULES];
int moduleCount = 0;

void registerModule(Module* mod) {
    if (moduleCount < MAX_MODULES) {
        moduleRegistry[moduleCount++] = mod;
        printf("Module '%s' registered.\n", mod->name);
    }
    else {
        printf("Module registry is full. Cannot register '%s'.\n", mod->name);
    }
}
