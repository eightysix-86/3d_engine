#include <stdlib.h>

#include "../include/math/vector.h"

/**
 * @brief A triplet of vertices index
 * 
 * Triplet of index representing a triangle for rasterization
 *
 * @field vert The triplet of int
 */
typedef struct Triangle {
    int vert[3];
} Triangle;

/**
 * @brief Mesh representation
 * 
 * array of vertices and array of triangles
 *
 * @field vertices Array of Vector4
 * @field triangles Array of Triangles
 */
typedef struct {
    Vector4* vertices;
    int vertex_count;

    Triangle* triangles;
    int triangle_count;
} Mesh;

Mesh mesh_generate(const Vector4* vertices, int vertex_count,
                   const Triangle* triangles, int triangle_count);
                   
void mesh_destroy(Mesh* mesh);