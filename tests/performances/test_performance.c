#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "core/pipeline.h"
#include "core/renderer.h"

// Performance measurement utilities
typedef struct {
    const char* name;
    double avg_time_ms;
    double min_time_ms;
    double max_time_ms;
    double stddev_time_ms;
    int iterations;
    int vertices;
    int triangles;
} PerformanceResult;

static PerformanceResult compute_stats(
    const char* name,
    double* samples,
    int count,
    int vertices,
    int triangles
) {
    double sum = 0.0, min = samples[0], max = samples[0];
    for (int i = 0; i < count; i++) {
        sum += samples[i];
        if (samples[i] < min) min = samples[i];
        if (samples[i] > max) max = samples[i];
    }
    double avg = sum / count;

    // Compute stddev
    double variance = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = samples[i] - avg;
        variance += diff * diff;
    }
    variance /= count;
    double stddev = sqrt(variance);

    PerformanceResult result = {
        .name = name,
        .avg_time_ms = avg,
        .min_time_ms = min,
        .max_time_ms = max,
        .stddev_time_ms = stddev,
        .iterations = count,
        .vertices = vertices,
        .triangles = triangles
    };
    return result;
}

static inline double get_time_ms(double start, double end) {
    Uint64 freq = SDL_GetPerformanceFrequency();
    return (double)((end - start) * 1000) / (double)freq;
}

// Test mesh generators
static Mesh* create_cube_mesh() {
    Vector4 vertices[] = {
        {-1.0f, -1.0f, -1.0f, 1.0f}, // 0
        { 1.0f, -1.0f, -1.0f, 1.0f}, // 1
        { 1.0f,  1.0f, -1.0f, 1.0f}, // 2
        {-1.0f,  1.0f, -1.0f, 1.0f}, // 3
        {-1.0f, -1.0f,  1.0f, 1.0f}, // 4
        { 1.0f, -1.0f,  1.0f, 1.0f}, // 5
        { 1.0f,  1.0f,  1.0f, 1.0f}, // 6
        {-1.0f,  1.0f,  1.0f, 1.0f}  // 7
    };

    Triangle triangles[] = {
        // Front face
        {{0, 1, 2}}, {{2, 3, 0}},
        // Back face
        {{4, 6, 5}}, {{6, 4, 7}},
        // Left face
        {{4, 0, 3}}, {{3, 7, 4}},
        // Right face
        {{1, 5, 6}}, {{6, 2, 1}},
        // Top face
        {{3, 2, 6}}, {{6, 7, 3}},
        // Bottom face
        {{4, 5, 1}}, {{1, 0, 4}}
    };

    return mesh_generate(vertices, 8, triangles, 12);
}

static Mesh* create_large_mesh(int subdivisions) {
    int vertex_count = (subdivisions + 1) * (subdivisions + 1);
    int triangle_count = subdivisions * subdivisions * 2;
    
    Vector4* vertices = malloc(sizeof(Vector4) * vertex_count);
    Triangle* triangles = malloc(sizeof(Triangle) * triangle_count);
    
    // Generate grid vertices
    int vertex_idx = 0;
    for (int i = 0; i <= subdivisions; i++) {
        for (int j = 0; j <= subdivisions; j++) {
            float x = (float)i / subdivisions * 2.0f - 1.0f;
            float z = (float)j / subdivisions * 2.0f - 1.0f;
            vertices[vertex_idx] = (Vector4){x, 0.0f, z, 1.0f};
            vertex_idx++;
        }
    }
    
    // Generate triangles
    int triangle_idx = 0;
    for (int i = 0; i < subdivisions; i++) {
        for (int j = 0; j < subdivisions; j++) {
            int base = i * (subdivisions + 1) + j;
            
            // First triangle
            triangles[triangle_idx].vert[0] = base;
            triangles[triangle_idx].vert[1] = base + 1;
            triangles[triangle_idx].vert[2] = base + subdivisions + 1;
            triangle_idx++;
            
            // Second triangle
            triangles[triangle_idx].vert[0] = base + 1;
            triangles[triangle_idx].vert[1] = base + subdivisions + 2;
            triangles[triangle_idx].vert[2] = base + subdivisions + 1;
            triangle_idx++;
        }
    }
    
    Mesh* mesh = mesh_generate(vertices, vertex_count, triangles, triangle_count);
    free(vertices);
    free(triangles);
    return mesh;
}

// Performance test for update_mesh
static PerformanceResult test_update_mesh_performance(Mesh* mesh, int iterations) {
    // Create clipped mesh with same structure
    Mesh* clipped = mesh_copy(mesh);
    
    // Setup test parameters
    Transform transform = {
        .translation = {0.0f, 0.0f, -5.0f},
        .scale = {1.0f, 1.0f, 1.0f},
        .rotation = {0.0f, 0.0f, 0.0f}
    };
    
    Camera camera = {
        .pos = {0.0f, 0.0f, 0.0f},
        .target = {0.0f, 0.0f, -1.0f},
        .up = {0.0f, 1.0f, 0.0f}
    };
    
    Projection projection = {
        .fov = M_PI / 4.0f,  // 45 degrees
        .aspect_ratio = 16.0f / 9.0f,
        .near = 0.1f,
        .far = 100.0f
    };
    
    // Warm up
    for (int i = 0; i < 10; i++) {
        update_mesh(mesh, clipped, transform, camera, projection);
    }
    
    // Measure performance
    double* samples = malloc(sizeof(double) * iterations);

    for (int i = 0; i < iterations; i++) {
        Uint64 t0 = SDL_GetPerformanceCounter();
        update_mesh(mesh, clipped, transform, camera, projection);
        Uint64 t1 = SDL_GetPerformanceCounter();
        samples[i] = get_time_ms(t0, t1);
    }

    PerformanceResult result = compute_stats(
        "update_mesh",
        samples,
        iterations,
        mesh->vertex_count,
        mesh->triangle_count
    );

    free(samples);
    return result;
}

// Performance test for draw_mesh
static PerformanceResult test_draw_mesh_performance(Mesh* mesh, int iterations) {
    // Initialize SDL for rendering
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return (PerformanceResult){"draw_mesh", -1.0, 0};
    }
    
    SDL_Window* window = SDL_CreateWindow("Performance Test",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        800, 600,
                                        SDL_WINDOW_HIDDEN);
    
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return (PerformanceResult){"draw_mesh", -1.0, 0};
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return (PerformanceResult){"draw_mesh", -1.0, 0};
    }
    
    // Setup draw parameters
    Draw draw_data = {
        .clipped_mesh = mesh,
        .color = {255, 255, 255, 255}
    };
    
    size_t screen_w = 800;
    size_t screen_h = 600;
    
    // Warm up
    for (int i = 0; i < 10; i++) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw_mesh(renderer, &draw_data, screen_w, screen_h);
        SDL_RenderPresent(renderer);
    }
    
    // Measure performance
    double* samples = malloc(sizeof(double) * iterations);

    for (int i = 0; i < iterations; i++) {
        Uint64 t0 = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw_mesh(renderer, &draw_data, screen_w, screen_h);
        SDL_RenderPresent(renderer);

        Uint64 t1 = SDL_GetPerformanceCounter();
        samples[i] = get_time_ms(t0, t1);
    }

    PerformanceResult result = compute_stats(
        "draw_mesh",
        samples,
        iterations,
        mesh->vertex_count,
        mesh->triangle_count
    );

    free(samples);
    return result;
}

static void print_performance_result(PerformanceResult result) {
    if (result.avg_time_ms < 0) {
        printf("❌ %s performance test failed\n", result.name);
        return;
    }

    printf("📊 %s Performance Results:\n", result.name);
    printf("   Iterations:   %d\n", result.iterations);
    printf("   Vertices:     %d\n", result.vertices);
    printf("   Triangles:    %d\n", result.triangles);
    printf("   Avg:          %.3f ms\n", result.avg_time_ms);
    printf("   Min:          %.3f ms\n", result.min_time_ms);
    printf("   Max:          %.3f ms\n", result.max_time_ms);
    printf("   Stddev:       %.3f ms\n", result.stddev_time_ms);
    printf("   Throughput:   %.1f calls/sec\n", 1000.0 / result.avg_time_ms);

    if (result.vertices > 0) {
        printf("   Vertices/sec: %.0f\n",
            (result.vertices * 1000.0) / result.avg_time_ms);
    }
    if (result.triangles > 0) {
        printf("   Triangles/sec: %.0f\n",
            (result.triangles * 1000.0) / result.avg_time_ms);
    }

    printf("\n");
}

static void print_summary_perf(
    PerformanceResult update,
    PerformanceResult draw,
    const char* mesh_name
) {
    double total = update.avg_time_ms + draw.avg_time_ms;

    printf("\n=== %s SUMMARY ===\n", mesh_name);
    printf("update_mesh: %.3f ms (%.1f%%)\n", update.avg_time_ms,
           update.avg_time_ms / total * 100.0);
    printf("draw_mesh:   %.3f ms (%.1f%%)\n", draw.avg_time_ms,
           draw.avg_time_ms / total * 100.0);
    printf("Total:       %.3f ms\n", total);
}

int main() {
    printf("=== MESH PERFORMANCE TESTS ===\n\n");
    
    // Test with different mesh sizes
    printf("Testing with cube mesh (8 vertices, 12 triangles):\n");
    Mesh* cube = create_cube_mesh();
    
    PerformanceResult cube_update = test_update_mesh_performance(cube, 10000);
    print_performance_result(cube_update);
    
    PerformanceResult cube_draw = test_draw_mesh_performance(cube, 1000);
    print_performance_result(cube_draw);
    
    mesh_destroy(cube);
    
    // Test with medium mesh
    printf("Testing with medium mesh (100x100 grid = 10,201 vertices, 20,000 triangles):\n");
    Mesh* medium_mesh = create_large_mesh(100);
    
    PerformanceResult medium_update = test_update_mesh_performance(medium_mesh, 1000);
    print_performance_result(medium_update);
    
    PerformanceResult medium_draw = test_draw_mesh_performance(medium_mesh, 100);
    print_performance_result(medium_draw);
    
    mesh_destroy(medium_mesh);
    
    // Test with large mesh
    printf("Testing with large mesh (200x200 grid = 40,401 vertices, 80,000 triangles):\n");
    Mesh* large_mesh = create_large_mesh(200);
    
    PerformanceResult large_update = test_update_mesh_performance(large_mesh, 100);
    print_performance_result(large_update);
    
    PerformanceResult large_draw = test_draw_mesh_performance(large_mesh, 10);
    print_performance_result(large_draw);
    
    mesh_destroy(large_mesh);
    
    // Summary
    print_summary_perf(cube_update, cube_draw, "Cube mesh");
    print_summary_perf(medium_update, medium_draw, "Medium mesh");
    print_summary_perf(large_update, large_draw, "Large mesh");
    
    return 0;
}
