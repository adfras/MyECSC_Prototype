#ifndef GRAVITY_COMPONENT_H
#define GRAVITY_COMPONENT_H

#include "components.h"
#include "ComponentArray.h"

typedef struct {
    Vec3 force;
} Gravity;

DEFINE_COMPONENT_ARRAY(Gravity);

#endif // GRAVITY_COMPONENT_H
