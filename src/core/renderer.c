#include <SDL.h>

#include "core/renderer.h"

static Pixel get_pixel_pos(Vector4 v_clip, int screen_w, int screen_h) {
    Pixel pos;

    pos.x = ((v_clip.x / v_clip.w) + 1) * 0.5 * screen_w;
    pos.y = (1 - (v_clip.y / v_clip.w)) * 0.5 * screen_h; // 1 - y_ndc to flip Y axis
    pos.z_depth = ((v_clip.z / v_clip.w) + 1) * 0.5; // [-1, 1] -> [0, 1] depth

    return pos;
}

void draw_mesh(SDL_Renderer* sdl_renderer, const Draw* figure, const size_t screen_w, const size_t screen_h) {
    SDL_SetRenderDrawColor(sdl_renderer, figure->color.r, figure->color.g, figure->color.b, figure->color.a);

    // Loop over triangles
    for (int i = 0; i < figure->mesh->triangle_count; i += 1) {
        int i0 = figure->mesh->triangles[i].vert[0];
        int i1 = figure->mesh->triangles[i].vert[1];
        int i2 = figure->mesh->triangles[i].vert[2];

        Pixel v0 = get_pixel_pos(figure->mesh->vertices[i0], screen_w, screen_h);
        Pixel v1 = get_pixel_pos(figure->mesh->vertices[i1], screen_w, screen_h);
        Pixel v2 = get_pixel_pos(figure->mesh->vertices[i2], screen_w, screen_h);

        // Draw triangle edges
        SDL_RenderDrawLine(sdl_renderer, v0.x, v0.y, v1.x, v1.y);
        SDL_RenderDrawLine(sdl_renderer, v1.x, v1.y, v2.x, v2.y);
        SDL_RenderDrawLine(sdl_renderer, v2.x, v2.y, v0.x, v0.y);
    }
}