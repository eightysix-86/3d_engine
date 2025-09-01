#include <SDL.h>
#include <stdio.h>

#include "engine.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define BLACK (Color){0, 0, 0, 255}
#define RED (Color){255, 0, 0, 255}

#define CAM_POS (Vector3){0.0f, 0.0f,  3.0f}
#define CAM_TARGET NULL_VECTOR3
#define CAM_UP (Vector3){0.0f, 1.0f,  0.0f}

int main(int argc, char *argv[]) {
    Engine* engine = engine_init("3d engine", WIN_WIDTH, WIN_HEIGHT, BLACK);
    if (engine == NULL) {
        printf("Error during engine creation: %s\n", SDL_GetError());
        return 1;
    }

    int running = 1;
    SDL_Event event;
    Transform draw_transform = {NULL_VECTOR3, (Vector3){1.0f, 1.0f, 1.0f}, NULL_VECTOR3};
    float rotation_speed = 60.0f * (M_PI / 180.0f); // 60°/s in radians
    float dt = 0.016f; // 16 ms frame (~60 FPS)
    float dx = rotation_speed * dt; // radians to rotate this frame

    // 8 unique cube vertices
    Vector4 cube_vertices[8] = {
        {-1, -1, -1, 1},  // 0
        { 1, -1, -1, 1},  // 1
        { 1,  1, -1, 1},  // 2
        {-1,  1, -1, 1},  // 3
        {-1, -1,  1, 1},  // 4
        { 1, -1,  1, 1},  // 5
        { 1,  1,  1, 1},  // 6
        {-1,  1,  1, 1}   // 7
    };

    // 12 triangles, each with 3 indices (two per cube face)
    Triangle cube_triangles[12] = {
        {0, 1, 2}, {0, 2, 3}, // back face
        {4, 5, 6}, {4, 6, 7}, // front face
        {0, 1, 5}, {0, 5, 4}, // bottom face
        {2, 3, 7}, {2, 7, 6}, // top face
        {0, 3, 7}, {0, 7, 4}, // left face
        {1, 2, 6}, {1, 6, 5}  // right face
    };

    Mesh* cube = mesh_generate(cube_vertices, 8, cube_triangles, 12);
    Camera cam = {
    .pos    = CAM_POS,   // placed a bit away from the origin
    .target = CAM_TARGET,   // looking at the origin
    .up     = CAM_UP    // Y axis is "up"
    };

    draw_init(engine, cube, RED, cam);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:    draw_transform.rotation.y -= dx; break;
                    case SDLK_DOWN:  draw_transform.rotation.y += dx; break;
                    case SDLK_LEFT:  draw_transform.rotation.x -= dx; break;
                    case SDLK_RIGHT: draw_transform.rotation.x += dx; break;
                }
            }
        }
        update_step(engine, draw_transform);
    }

    engine_destroy(engine);
    return 0;
}
