#pragma once
// Components.h
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdint.h>

// Define the Entity type.
typedef uint32_t Entity;

// Define a 3D vector.
typedef struct {
    float x;
    float y;
    float z;
} Vec3;

// Define a quaternion.
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Quat;

// Define the Transform component.
typedef struct {
    Vec3 position;
    Quat rotation;
    Vec3 scale;
} Transform;

#endif // COMPONENTS_H

