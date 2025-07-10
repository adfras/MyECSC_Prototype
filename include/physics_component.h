// physics_component.h

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "components.h" // for Vec3
#include "ComponentArray.h" 

// Define the physics component
typedef struct {
	Vec3 velocity;
	Vec3 force;
} Physics;

// Create a physics component array
DEFINE_COMPONENT_ARRAY(Physics);

#endif // !PHYSICS_COMPONENT_H
