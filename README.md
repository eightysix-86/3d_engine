# 3D Engine

A lightweight 3D graphics engine written in C using SDL2 for rendering. This engine provides a complete 3D rendering pipeline with support for mesh rendering, transformations, camera controls, and perspective projection.

## Features

- **3D Mesh Rendering**: Support for triangle-based 3D meshes with vertex and index data
- **3D Transformations**: Translation, rotation, and scaling operations
- **Camera System**: Configurable camera with position, target, and up vector
- **Perspective Projection**: Field of view based perspective projection with near/far clipping
- **Real-time Rendering**: 60 FPS rendering loop with SDL2 backend
- **Interactive Controls**: Keyboard controls for object rotation
- **Memory Management**: Proper allocation and cleanup of 3D resources
- **Comprehensive Testing**: Unit tests for core mathematical operations

## Dependencies

- **SDL2**: Graphics rendering and window management
- **GCC**: C compiler with C11 support
- **Make**: Build system

### Installing Dependencies

**macOS (Homebrew):**
```bash
brew install sdl2
```

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl2-dev
```

**Fedora:**
```bash
sudo dnf install SDL2-devel
```

## Make Commands

| Command              | Description                                                                 |
|----------------------|-----------------------------------------------------------------------------|
| `make` / `make all`  | Build the engine (default target).                                          |
| `make run`           | Clean, build, and run the engine.                                           |
| `make clean`         | Remove all build artifacts (`.o`, `.d`, and `build/` directory).            |
| `make test`          | Build and run all tests from `tests/`.                 |
| `make test-<name>`   | Build and run a specific test file (`tests/test_<name>.c`).                 |
| `make perf`          | Build and run all performance tests from `tests/performances/`.             |

### Examples

```bash
make run                 # Run the engine
make test-pipeline       # Run pipeline tests only
make perf                # Run all performance benchmarks
```


## Usage

### Engine Functions

- **`engine_init(title, width, height, background_color)`**  
  Initializes SDL, creates the window and renderer, and prepares the engine context.  
  Returns an `Engine*`.

- **`mesh_generate(vertices, vertex_count, triangles, triangle_count)`**  
  Creates a `Mesh` object from raw vertex and triangle arrays.

- **`draw_init(engine, mesh, color, camera)`**  
  Prepares the engine to draw a specific mesh with a given color and camera.

- **`update_step(engine, transform)`**  
  Runs one frame of the pipeline: applies the transform, updates the mesh through the camera and projection, and renders to the window.

- **`engine_destroy(engine)`**  
  Cleans up all allocated resources (SDL renderer, window, engine memory).

### Creating Custom Meshes

```c
// Define vertices (homogeneous coordinates)
Vector4 cube_vertices[8] = {
    {-1, -1, -1, 1}, {1, -1, -1, 1},  // Back face
    {1,  1, -1, 1},  {-1, 1, -1, 1},
    {-1, -1,  1, 1}, {1, -1,  1, 1},  // Front face
    {1,  1,  1, 1},  {-1, 1,  1, 1}
};

// Define triangles (vertex indices)
Triangle cube_triangles[12] = {
    {0, 1, 2}, {0, 2, 3},  // Back face
    {4, 5, 6}, {4, 6, 7},  // Front face
    // ... more faces
};

Mesh* cube = mesh_generate(cube_vertices, 8, cube_triangles, 12);
```

### Applying Transformations

```c
Transform transform = {
    .translation = {0.0f, 0.0f, 0.0f},
    .rotation = {0.0f, 45.0f * (M_PI/180.0f), 0.0f},  // 45° Y rotation
    .scale = {1.0f, 1.0f, 1.0f}
};

update_step(engine, transform);
```

### Example

This example shows how to set up the engine, create a cube mesh, and render it with basic transformations.

```c
#include <SDL.h>
#include <stdio.h>
#include "engine.h"

#define WIN_WIDTH 600
#define WIN_HEIGHT 600
#define BLACK (Color){0, 0, 0, 255}
#define RED   (Color){255, 0, 0, 255}

int main(int argc, char *argv[]) {
    // 1️⃣ Initialize the engine
    Engine* engine = engine_init("3D Engine", WIN_WIDTH, WIN_HEIGHT, BLACK);
    if (!engine) {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    // 2️⃣ Define and generate mesh (vertices + triangles)
    Vector4 cube_vertices[8] = {
        {-1, -1, -1, 1}, {1, -1, -1, 1}, {1, 1, -1, 1}, {-1, 1, -1, 1},
        {-1, -1,  1, 1}, {1, -1,  1, 1}, {1, 1,  1, 1}, {-1, 1,  1, 1}
    };

    Triangle cube_triangles[12] = {
        {0, 1, 2}, {0, 2, 3}, {4, 5, 6}, {4, 6, 7}, // front/back
        {0, 1, 5}, {0, 5, 4}, {2, 3, 7}, {2, 7, 6}, // top/bottom
        {0, 3, 7}, {0, 7, 4}, {1, 2, 6}, {1, 6, 5}  // sides
    };

    Mesh* cube = mesh_generate(cube_vertices, 8, cube_triangles, 12);

    // 3️⃣ Setup camera
    Camera cam = {
        .pos    = {0.0f, 0.0f, 5.0f},   // position in world
        .target = {0.0f, 0.0f, 0.0f},   // looking at the origin
        .up     = {0.0f, 1.0f, 0.0f}    // Y-axis is "up"
    };

    // 4️⃣ Initialize the drawing pipeline
    draw_init(engine, cube, RED, cam);

    // 5️⃣ Main loop
    int running = 1;
    SDL_Event event;
    Transform draw_transform = NO_TRANSFORM;

    while (running) {
        // Handle input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            
            /* Inputs */
        }

        /* How you want to update your mesh (rotation, translation, etc) */

        // 6️⃣ Render one frame with given transformations
        update_step(engine, draw_transform);
    }

    // 7️⃣ Cleanup
    engine_destroy(engine);
    return 0;
}
```

## Demo Application

The included demo (`main.c`) demonstrates:

- **Rotating Cube**: A red cube that continuously rotates around XYZ-axes
- **Perspective Projection**: 3D perspective with proper depth

### Controls
- **↑/↓**: Rotate around X-axis
- **←/→**: Rotate around Y-axis
- **W/S**: Move along Z-axis
- **A/D**: Move along X-axis
- **Q/E**: Move along Y-axis
- **Y/X**: Scale X-axis up/down
- **C/V**: Scale Y-axis up/down
- **B/N**: Scale Z-axis up/down
- **ESC**: Exit application

### Demo
https://github.com/user-attachments/assets/1af29484-0dfb-4eda-984d-be7122e6d386

## Core Components

### Engine (`engine.c`)
- SDL2 initialization and window management
- Main rendering loop coordination
- Resource management and cleanup

### Mesh System (`mesh.c`)
- Triangle-based 3D mesh representation
- Dynamic memory management for vertices and triangles
- Mesh copying and destruction utilities

### 3D Pipeline (`pipeline.c`)
- Model-View-Projection matrix transformations
- Camera positioning and orientation
- Perspective projection with configurable parameters

### Renderer (`renderer.c`)
- SDL2-based triangle rasterization
- Screen space coordinate conversion
- Wireframe rendering

### Transformations (`transform.c`)
- Translation, rotation, and scaling matrices
- Support for X, Y, Z axis rotations
- Matrix composition for complex transformations

### Math Library (`math/`)
- **Vectors**: 3D and 4D vector operations with generic macros
- **Matrices**: 4x4 matrix multiplication and transformations
- **Utilities**: Dot product, cross product, normalization


## Future Enhancements

Potential areas for expansion:
- Texture mapping and UV coordinates
- Lighting and shading models
- Z-buffer depth testing
- Solid triangle filling
- Model loading (OBJ, PLY formats)
- Multiple mesh rendering
- Advanced camera controls (FPS, orbit)

## License
This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass with `make test_all`
5. Submit a pull request

## Troubleshooting

**⚠️ This project has only been tested on a Mac with ARM chip !**
