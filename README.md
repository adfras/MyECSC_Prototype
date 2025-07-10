# MyECSC Prototype

MyECSC is a small Entity Component System (ECS) written in C. The goal of the project is to provide a minimal example of how an ECS can be implemented and used in a C application.

## Getting the Source

Clone the repository and change into the project directory:

```bash
git clone https://github.com/adfras/MyECSC_Prototype.git
cd MyECSC_Prototype
```

## Building and Running

A helper script `run.sh` is included to build and execute the demo program. It will attempt to use SDL3 if the development libraries are detected, otherwise it builds a headless version.

```bash
./run.sh
```

The script compiles the sources in `src/` with `gcc` and produces the `MyECSC_Prototype` executable which is then run automatically.

## Tests

The `tests` directory contains a simple test for the entity manager. To build and run the test manually, execute:

```bash
gcc -std=c11 -Iinclude src/entity_manager.c tests/entity_manager_test.c -o entity_manager_test
./entity_manager_test
```

If all assertions succeed the program prints `"Entity manager test passed"`.

## Repository Layout

- Source files are in `src/` and headers are in `include/`.
- `tests/` contains unit tests.
- `run.sh` builds and runs the demo.

All build artefacts and generated files are ignored via `.gitignore`.
