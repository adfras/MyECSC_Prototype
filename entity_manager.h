#pragma once
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

// Define the maximum number of entities
#define MAX_ENTITIES 10000

// Define an entity as an unsigned 32-bit integer

typedef uint32_t Entity;

// Define a signature as an unsigned 32-bit integer

typedef uint32_t Signature;

// The EntityManager holds available IDs, an array of signatures (one for each entity), and a counts
typedef struct
{
	Entity availableEntities[MAX_ENTITIES];
	int head;
	int tail;
	int count;
	Signature signatures[MAX_ENTITIES]; // each entity has a signature
	uint32_t LivingEntityCount;
} EntityManager;


// Initialize the EntityManager
void EntityManager_Init(EntityManager *manager);

// Create a new entity by returning the next available ID
Entity EntityManager_CreateEntity(EntityManager *manager);

// Destroy an entity by adding its ID to the available IDs
void EntityManager_DestroyEntity(EntityManager *manager, Entity entity);

// Set the signature of an entity
void EntityManager_SetSignature(EntityManager *manager, Entity entity, Signature signature);

// Get the signature of an entity
Signature EntityManager_GetSignature(EntityManager *manager, Entity entity);


#endif // !ENTITY_MANAGER_H

