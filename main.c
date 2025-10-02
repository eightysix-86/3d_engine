#include <SDL.h>
#include <stdio.h>

#include "engine.h"

#define SHOW_FPS 1

#define WIN_WIDTH 600
#define WIN_HEIGHT 600

#define BLACK (Color){0, 0, 0, 255}
#define RED (Color){255, 0, 0, 255}

#define CAM_POS (Vector3){0.0f, 0.0f, 5.0f}
#define CAM_TARGET (Vector3){0.0f, 0.0f, 0.0f}
#define CAM_UP (Vector3){0.0f, 1.0f,  0.0f}

#define ROTATION_SPEED 45.0f // in degree
#define TRANSLATION_SPEED 5.0f
#define SCALE_SPEED 1.5f

int main(int argc, char *argv[]) {
    Engine* engine = engine_init("3d engine", WIN_WIDTH, WIN_HEIGHT, BLACK);
    if (engine == NULL) {
        printf("Error during engine creation: %s\n", SDL_GetError());
        return 1;
    }

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
    
    int running = 1;
    SDL_Event event;
    Transform draw_transform = NO_TRANSFORM;
    float dr = 0.0f;
    float ds = 0.0f;
    float dz = 0.0f;

    Uint32 last_time = SDL_GetTicks();  // time at previous frame
    Uint32 last_fps_time = SDL_GetTicks();
    int frames = 0;
    float fps = 0.0f;

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed = current_time - last_time;
        float delta_time = elapsed / 1000.0f;

        dr = ROTATION_SPEED * (M_PI / 180.0f) * delta_time;
        ds = TRANSLATION_SPEED * delta_time;
        dz = SCALE_SPEED * delta_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: running = 0; break;
                    case SDLK_UP:    draw_transform.rotation.x -= dr; break;
                    case SDLK_DOWN:  draw_transform.rotation.x += dr; break;
                    case SDLK_LEFT:  draw_transform.rotation.y -= dr; break;
                    case SDLK_RIGHT: draw_transform.rotation.y += dr; break;
                    case SDLK_w:    draw_transform.translation.z -= ds; break;
                    case SDLK_s:    draw_transform.translation.z += ds; break;
                    case SDLK_d:    draw_transform.translation.x -= ds; break;
                    case SDLK_a:    draw_transform.translation.x += ds; break;
                    case SDLK_q:    draw_transform.translation.y -= ds; break;
                    case SDLK_e:    draw_transform.translation.y += ds; break;
                    case SDLK_y:    draw_transform.scale.x += dz; break;
                    case SDLK_x:    draw_transform.scale.x -= dz; break;
                    case SDLK_c:    draw_transform.scale.y += dz; break;
                    case SDLK_v:    draw_transform.scale.y -= dz; break;
                    case SDLK_b:    draw_transform.scale.z += dz; break;
                    case SDLK_n:    draw_transform.scale.z -= dz; break;
                }
            }
        }

        draw_transform.rotation.x += dr;
        draw_transform.rotation.y += dr;
        draw_transform.rotation.z += dr;

        update_step(engine, draw_transform);
        
        // FPS calculation and display every second
        if (SHOW_FPS) {
            frames++;
            Uint32 fps_elapsed = current_time - last_fps_time;
            if (fps_elapsed >= 1000) {
                fps = frames * 1000.0f / fps_elapsed;
                frames = 0;
                last_fps_time = current_time;
                printf("FPS: %.2f\n", fps);
            }
        }

        // Update last_time every frame for smooth delta time
        last_time = current_time;
    }

    engine_destroy(engine);
    return 0;
}
