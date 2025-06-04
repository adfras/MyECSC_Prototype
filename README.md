# MyECSC Prototype

This repository contains a simple ECS (Entity Component System) prototype.

## Tests

A small test program is provided in `tests/entity_manager_test.c` to verify the
basic functionality of `EntityManager`.

### Building the test
Compile the test with GCC:

```bash
gcc -std=c11 -I. entity_manager.c tests/entity_manager_test.c -o entity_manager_test
./entity_manager_test
```

The program will output `"Entity manager test passed"` if all assertions
succeed.
