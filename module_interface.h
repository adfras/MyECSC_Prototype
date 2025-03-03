#ifndef MODULE_INTERFACE_H
#define MODULE_INTERFACE_H

#include "coordinator.h"

// All modules must export a function with this signature.
typedef void (*ModuleRegisterFunc)(Coordinator* coordinator);

#endif // MODULE_INTERFACE_H
