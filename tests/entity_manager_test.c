#include "entity_manager.h"
#include <assert.h>
#include <stdio.h>

/*
Build instructions:
    gcc -std=c11 -I. entity_manager.c tests/entity_manager_test.c -o entity_manager_test
*/
int main(void) {
    EntityManager manager;
    EntityManager_Init(&manager);
    assert(manager.count == MAX_ENTITIES);
    assert(manager.LivingEntityCount == 0);

    int initialCount = manager.count;
    int initialLiving = manager.LivingEntityCount;

    Entity e = EntityManager_CreateEntity(&manager);

    assert(manager.count == initialCount - 1);
    assert(manager.LivingEntityCount == initialLiving + 1);

    EntityManager_DestroyEntity(&manager, e);

    assert(manager.count == initialCount);
    assert(manager.LivingEntityCount == initialLiving);

    printf("Entity manager test passed\n");
    return 0;
}
