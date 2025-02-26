#include "physics_system.h"
#include <assert.h>

void PhysicsSystem_Init(PhysicsSystem* psys, ComponentManager* cm) {
    psys->base.count = 0;
    // Set the required signature: (1 << COMPONENT_TRANSFORM) for Transform and (1 << COMPONENT_PHYSICS) for Physics.
    psys->base.requiredSignature = (1 << COMPONENT_TRANSFORM) | (1 << COMPONENT_PHYSICS);
    psys->componentManager = cm;
}

void PhysicsSystem_Update(PhysicsSystem* psys, float dt) {
    // Retrieve the component arrays from the ComponentManager.
    TransformComponentArray* transformArray = (TransformComponentArray*)psys->componentManager->componentArrays[COMPONENT_TRANSFORM];
    PhysicsComponentArray* physicsArray = (PhysicsComponentArray*)psys->componentManager->componentArrays[COMPONENT_PHYSICS];

    for (int i = 0; i < psys->base.count; i++) {
        Entity entity = psys->base.entities[i];

        // Retrieve the Transform and Physics components.
        Transform* transform = TransformComponentArray_GetData(transformArray, entity);
        Physics* physics = PhysicsComponentArray_GetData(physicsArray, entity);

        // Update position: position += velocity * dt.
        transform->position.x += physics->velocity.x * dt;
        transform->position.y += physics->velocity.y * dt;
        transform->position.z += physics->velocity.z * dt;

        // Update velocity: velocity += force * dt.
        physics->velocity.x += physics->force.x * dt;
        physics->velocity.y += physics->force.y * dt;
        physics->velocity.z += physics->force.z * dt;
    }
}