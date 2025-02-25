#pragma once

// TransformComponent.h
#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "ComponentArray.h"
#include "Components.h"  // This should include the definition of Transform

// Expand the macro to generate the Transform component array.
DEFINE_COMPONENT_ARRAY(Transform)

#endif // TRANSFORM_COMPONENT_H
