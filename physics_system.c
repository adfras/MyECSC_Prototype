#include "physics_system.h"
#include <assert.h>

void PhysicsSystem_Init(PhysicsSystem* psys, ComponentManager* cm) {
    psys->base.count = 0;
    psys->base.requiredSignature = (1 << COMPONENT_TRANSFORM) |
        (1 << COMPONENT_RIGID_BODY) |
        (1 << COMPONENT_GRAVITY);
    psys->componentManager = cm;
}

void PhysicsSystem_Update(PhysicsSystem* psys, float dt) {
    // Retrieve the component arrays from the ComponentManager.
    TransformComponentArray* transformArray = (TransformComponentArray*)psys->componentManager->componentArrays[COMPONENT_TRANSFORM];
    RigidBodyComponentArray* rigidBodyArray = (RigidBodyComponentArray*)psys->componentManager->componentArrays[COMPONENT_RIGID_BODY];
    GravityComponentArray* gravityArray = (GravityComponentArray*)psys->componentManager->componentArrays[COMPONENT_GRAVITY];

    for (int i = 0; i < psys->base.count; i++) {
        Entity entity = psys->base.entities[i];

        Transform* transform = TransformComponentArray_GetData(transformArray, entity);
        RigidBody* rigidBody = RigidBodyComponentArray_GetData(rigidBodyArray, entity);
        Gravity* gravity = GravityComponentArray_GetData(gravityArray, entity);

        // Update the position using the current velocity.
        transform->position.x += rigidBody->velocity.x * dt;
        transform->position.y += rigidBody->velocity.y * dt;
        transform->position.z += rigidBody->velocity.z * dt;

        // Update the velocity based on gravity.
        rigidBody->velocity.x += gravity->force.x * dt;
        rigidBody->velocity.y += gravity->force.y * dt;
        rigidBody->velocity.z += gravity->force.z * dt;
    }
}
