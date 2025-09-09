#include "math/matrix.h"
#include "core/mesh.h"

typedef struct Pixel { 
    int x, y;
    float z_depth; 
} Pixel;

typedef struct Color { 
    size_t r, g, b, a; 
} Color;

typedef struct Draw { 
    Mesh* clipped_mesh;
    Color color;
} Draw;

void draw_mesh(SDL_Renderer* sdl_renderer, const Draw* figure, const size_t screen_w, const size_t screen_h);
