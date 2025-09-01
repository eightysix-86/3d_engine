#include "core/mesh.h"

Mesh* mesh_generate(const Vector4* vertices, int vertex_count,
                   const Triangle* triangles, int triangle_count) {
    Mesh* mesh = malloc(sizeof(Mesh));
    if (!mesh) return NULL;
    
    mesh->vertex_count = vertex_count;
    mesh->triangle_count = triangle_count;

    // allocate internal storage
    mesh->vertices = malloc(sizeof(Vector4) * vertex_count);
    mesh->triangles = malloc(sizeof(Triangle) * triangle_count);

    // copy input data
    for (int i = 0; i < vertex_count; i++) {
        mesh->vertices[i] = vertices[i];
    }
    for (int i = 0; i < triangle_count; i++) {
        mesh->triangles[i] = triangles[i];
    }

    return mesh;
}

Mesh* mesh_copy(const Mesh* src) {
    Mesh* dst = malloc(sizeof(Mesh));
    if (!dst) return NULL;

    dst->vertex_count = src->vertex_count;
    dst->vertices = malloc(sizeof(Vector4) * src->vertex_count);
    for (int i = 0; i < src->vertex_count; i++)
        dst->vertices[i] = src->vertices[i];

    dst->triangle_count = src->triangle_count;
    dst->triangles = malloc(sizeof(Triangle) * src->triangle_count);
    for (int i = 0; i < src->triangle_count; i++)
        dst->triangles[i] = src->triangles[i];

    return dst;
}

void mesh_destroy(Mesh* mesh) {
    if (mesh->vertices) free(mesh->vertices);
    if (mesh->triangles) free(mesh->triangles);
    free(mesh);
}