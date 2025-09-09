#include "engine.h"

#define FOV (M_PI / 3)
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

Engine* engine_init(const char* title, const int w, const int h, Color background) {
    Engine* engine = malloc(sizeof(Engine));
    if (!engine) return NULL;

    engine->draw = malloc(sizeof(Draw));
    if (!engine->draw) return NULL;

    engine->background = background;
    engine->screen_w = w;
    engine->screen_h = h;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return NULL;
    }

    engine->window = SDL_CreateWindow(title, 0, 0, w, h, SDL_WINDOW_SHOWN);
    if (engine->window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    engine->sdl_renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (engine->sdl_renderer == NULL) {
        SDL_DestroyWindow(engine->window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    return engine;
}

void draw_init(Engine* engine, const Mesh* mesh, Color color, Camera cam) {
    engine->figure = mesh;
    engine->draw->clipped_mesh = mesh_copy(mesh);
    engine->draw->color = color;
    
    engine->projection = (Projection){
        .fov          = FOV,
        .aspect_ratio = engine->screen_w / engine->screen_h,
        .near         = NEAR_PLANE,
        .far          = FAR_PLANE
    };

    engine->camera = cam;
}

void update_step(Engine* engine, Transform draw_transform) {
    SDL_SetRenderDrawColor(engine->sdl_renderer, 
        engine->background.r, engine->background.g, engine->background.b, engine->background.a); // background color
    SDL_RenderClear(engine->sdl_renderer);
    SDL_SetRenderDrawColor(engine->sdl_renderer, 
        engine->draw->color.r, engine->draw->color.g, engine->draw->color.b, engine->draw->color.a); // draw color

    update_mesh(engine->figure, engine->draw->clipped_mesh, draw_transform, engine->camera, engine->projection);
    draw_mesh(engine->sdl_renderer, engine->draw, engine->screen_w, engine->screen_h);
    
    SDL_RenderPresent(engine->sdl_renderer);
}

void engine_destroy(Engine* engine) {
    SDL_DestroyRenderer(engine->sdl_renderer);
    SDL_DestroyWindow(engine->window);
    SDL_Quit();

    mesh_destroy(engine->figure);
    mesh_destroy(engine->draw->clipped_mesh);
    free(engine->draw);
    free(engine);
}
