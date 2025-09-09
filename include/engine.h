#include <SDL.h>

#include "core/renderer.h"
#include "core/pipeline.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* sdl_renderer;

    Mesh* figure;
    Draw* draw;

    int screen_w;
    int screen_h;

    Camera camera;
    Projection projection;

    Color background;
} Engine;

Engine* engine_init(const char* title, int w, int h, Color background);

void draw_init(Engine* engine, const Mesh* mesh, Color color, Camera cam);

void update_step(Engine* engine, Transform draw_transform);

void engine_destroy(Engine* engine);