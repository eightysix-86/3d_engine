/**
 * @brief A 4D vector type for homogeneous coordinates in 3D graphics
 *
 * This type represents a 4D vector using homogeneous coordinates,
 * typically for positions or directions in 3D space.
 *
 * It provides two access modes:
 * - Named components: .x, .y, .z, .w
 * - Array indexing: v[0..3]
 *
 * Usage:
 *   Vector4 p = { .x = 1.0f, .y = 2.0f, .z = 3.0f, .w = 1.0f }; // point
 *   Vector4 d = { .x = 0.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f }; // direction
 *
 * @field x The x component
 * @field y The y component
 * @field z The z component
 * @field w The w component (1 for points, 0 for directions)
 * @field v Alternative array access (v[0]=x, v[1]=y, v[2]=z, v[3]=w)
 */

typedef union {
    struct { float x, y, z, w; };
    float v[4];
} Vector4;


/**
 * @brief Adds two 4D vectors together
 * @param v1 The first Vector4 to add
 * @param v2 The second Vector4 to add
 * @return The resulting Vector4 after addition of v1 and v2
 */
Vector4 add(Vector4 v1, Vector4 v2);

/**
 * @brief Subtracts two 4D vectors
 * @param v1 First Vector4 (minuend)
 * @param v2 Second Vector4 (subtrahend)
 * @return Vector4 Result of v1 - v2
 */
Vector4 subtract(Vector4 v1, Vector4 v2);

/**
 * @brief Calculates the dot product of two 4D vectors
 * @param v1 First Vector4 operand
 * @param v2 Second Vector4 operand
 * @return float Returns the scalar result of the dot product
 */
float dot(Vector4 v1, Vector4 v2);

/**
 * @brief Converts a float array to a Vector4
 * @param arr Array of 4 float values to be converted
 * @return Vector4 containing the values from the input array
 * 
 * Takes a float array of size 4 and returns a Vector4 object initialized with
 * the values from the array. The array elements are mapped to vector components
 * in order (x,y,z,w).
 */
Vector4 array_to_vect(float arr[4]);
