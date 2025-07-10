#include "entity_manager.h"

void EntityManager_Init(EntityManager *manager) {
    for (Entity i = 0; i < MAX_ENTITIES; i++) {
        manager->availableEntities[i] = i;
        manager->signatures[i] = 0; // No components initially.
    }
    manager->head = 0;
    manager->tail = 0; // When the queue is full, head == tail.
    manager->count = MAX_ENTITIES;
    manager->LivingEntityCount = 0;
}

Entity EntityManager_CreateEntity(EntityManager *manager) {
	assert(manager->LivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
	assert(manager->count > 0 && "Ran out of available entities.");

	// Take an ID from the front of the queue.
	Entity id = manager->availableEntities[manager->head];
	manager->head = (manager->head + 1) % MAX_ENTITIES;
	manager->count--;
	manager->LivingEntityCount++;
	return id;
}

void EntityManager_DestroyEntity(EntityManager *manager, Entity entity) {
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	// Invalidate the destroyed entity's signature.
	manager->signatures[entity] = 0;
	// Put the destroyed ID at the back of the queue.
	manager->availableEntities[manager->tail] = entity;
	manager->tail = (manager->tail + 1) % MAX_ENTITIES;
	manager->count++;
	manager->LivingEntityCount--;
}

void EntityManager_SetSignature(EntityManager *manager, Entity entity, Signature signature) {
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	manager->signatures[entity] = signature;
}

Signature EntityManager_GetSignature(EntityManager *manager, Entity entity) {
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	return manager->signatures[entity];
}