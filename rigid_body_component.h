#ifndef RIGID_BODY_COMPONENT_H
#define RIGID_BODY_COMPONENT_H

#include "components.h"
#include "ComponentArray.h"

typedef struct {
    Vec3 velocity;
    Vec3 acceleration;
} RigidBody;

DEFINE_COMPONENT_ARRAY(RigidBody);

#endif // RIGID_BODY_COMPONENT_H
