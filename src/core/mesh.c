#include ".././include/core/mesh.h"

Mesh mesh_generate(const Vector4* vertices, int vertex_count,
                   const Triangle* triangles, int triangle_count) 
{
    Mesh mesh;
    mesh.vertex_count = vertex_count;
    mesh.triangle_count = triangle_count;

    // allocate internal storage
    mesh.vertices = malloc(sizeof(Vector4) * vertex_count);
    mesh.triangles = malloc(sizeof(Triangle) * triangle_count);

    // copy input data
    for (int i = 0; i < vertex_count; i++) {
        mesh.vertices[i] = vertices[i];
    }
    for (int i = 0; i < triangle_count; i++) {
        mesh.triangles[i] = triangles[i];
    }

    return mesh;
}


void mesh_destroy(Mesh* mesh) {
    if (mesh->vertices) free(mesh->vertices);
    if (mesh->triangles) free(mesh->triangles);
    mesh->vertices = NULL;
    mesh->triangles = NULL;
    mesh->vertex_count = 0;
    mesh->triangle_count = 0;
}