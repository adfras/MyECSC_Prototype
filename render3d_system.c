#include "render3d_system.h"
#include "TransformComponent.h"
#include "coordinator.h"
#include "components.h"
#include <math.h>

// Access the global entityColors array from main.c
extern SDL_Color entityColors[MAX_ENTITIES];

// --- 3D Math Helpers ---
static inline Vec3 vec3_add(Vec3 a, Vec3 b) {
    Vec3 result = { a.x + b.x, a.y + b.y, a.z + b.z };
    return result;
}

static inline Vec3 vec3_scale(Vec3 v, float s) {
    Vec3 result = { v.x * s, v.y * s, v.z * s };
    return result;
}

// Rotate v around X, then Y, then Z (Euler order X->Y->Z)
static inline Vec3 vec3_rotate_xyz(Vec3 v, float rx, float ry, float rz) {
    // Rotate around X
    float cosX = cosf(rx), sinX = sinf(rx);
    Vec3 rxV = {
        v.x,
        v.y * cosX - v.z * sinX,
        v.y * sinX + v.z * cosX
    };
    // Rotate around Y
    float cosY = cosf(ry), sinY = sinf(ry);
    Vec3 rxyV = {
        rxV.x * cosY + rxV.z * sinY,
        rxV.y,
        -rxV.x * sinY + rxV.z * cosY
    };
    // Rotate around Z
    float cosZ = cosf(rz), sinZ = sinf(rz);
    Vec3 rxyzV = {
        rxyV.x * cosZ - rxyV.y * sinZ,
        rxyV.x * sinZ + rxyV.y * cosZ,
        rxyV.z
    };
    return rxyzV;
}

// --- Projection Helper ---
static inline void projectPoint(const Vec3* point, float fov, float viewerDistance,
    float aspect,
    float* outX, float* outY)
{
    float factor = fov / (viewerDistance + point->z);
    *outX = (point->x * factor) * aspect; 
    *outY = point->y * factor;
}

// A cube has 8 corners -> 12 triangles -> 36 indices
// We'll define them in (triangle list) form, each face as two triangles:
static const int cubeIndices[36] = {
    // Back face (0,1,2,3)
    0,1,2, 2,3,0,
    // Front face (4,5,6,7)
    4,5,6, 6,7,4,
    // Left face (0,3,7,4)
    0,3,7, 7,4,0,
    // Right face (1,2,6,5)
    1,2,6, 6,5,1,
    // Top face (3,2,6,7)
    3,2,6, 6,7,3,
    // Bottom face (0,1,5,4)
    0,1,5, 5,4,0
};

// Local positions for a unit cube from -0.5..0.5 in each axis
static const Vec3 localCubeVerts[8] = {
    { -0.5f, -0.5f, -0.5f }, // 0
    {  0.5f, -0.5f, -0.5f }, // 1
    {  0.5f,  0.5f, -0.5f }, // 2
    { -0.5f,  0.5f, -0.5f }, // 3
    { -0.5f, -0.5f,  0.5f }, // 4
    {  0.5f, -0.5f,  0.5f }, // 5
    {  0.5f,  0.5f,  0.5f }, // 6
    { -0.5f,  0.5f,  0.5f }  // 7
};

// Renders a solid cube by sending transformed vertices to SDL_RenderGeometry
static void renderSolidCube(SDL_Renderer* renderer,
    Vec3 center,
    float rotX, float rotY, float rotZ,
    float scale,
    SDL_Color color,
    float fov, float viewerDistance,
    int screenWidth, int screenHeight)
{
    // Transform and project each of the 8 corners
    SDL_Vertex vertices[8];

    float aspect = (float)screenWidth / (float)screenHeight;

    for (int i = 0; i < 8; i++) {
        // Start with the local vertex
        Vec3 v = localCubeVerts[i];
        // Apply uniform scale.
        v.x *= scale; v.y *= scale; v.z *= scale;

        // Rotate around X axis.
        {
            float cosX = cosf(rotX), sinX = sinf(rotX);
            float y = v.y * cosX - v.z * sinX;
            float z = v.y * sinX + v.z * cosX;
            v.y = y; v.z = z;
        }
        // Rotate around Y axis.
        {
            float cosY = cosf(rotY), sinY = sinf(rotY);
            float x = v.x * cosY + v.z * sinY;
            float z = -v.x * sinY + v.z * cosY;
            v.x = x; v.z = z;
        }
        // Rotate around Z axis.
        {
            float cosZ = cosf(rotZ), sinZ = sinf(rotZ);
            float x = v.x * cosZ - v.y * sinZ;
            float y = v.x * sinZ + v.y * cosZ;
            v.x = x; v.y = y;
        }
        // Translate to the cube's center.
        v = (Vec3){ v.x + center.x, v.y + center.y, v.z + center.z };

        // Project to 2D using the updated projectPoint.
        float projX, projY;
        projectPoint(&v, fov, viewerDistance, aspect, &projX, &projY);
        vertices[i].position.x = projX + screenWidth / 2.0f;
        vertices[i].position.y = -projY + screenHeight / 2.0f;

        // Convert SDL_Color (0–255) to SDL_FColor (0–1).
        vertices[i].color.r = color.r / 255.0f;
        vertices[i].color.g = color.g / 255.0f;
        vertices[i].color.b = color.b / 255.0f;
        vertices[i].color.a = color.a / 255.0f;

        vertices[i].tex_coord.x = 0;
        vertices[i].tex_coord.y = 0;
    }

    SDL_RenderGeometry(renderer, NULL, vertices, 8, cubeIndices, 36);

}

// --- Render3DSystem Functions ---
void Render3DSystem_Init(Render3DSystem* r3dSys, ComponentManager* cm) {
    // Only requires the Transform component
    r3dSys->base.count = 0;
    r3dSys->base.requiredSignature = (1 << COMPONENT_TRANSFORM);
    r3dSys->componentManager = cm;
}

void Render3DSystem_Update(Render3DSystem* r3dSys, float dt,
    SDL_Renderer* renderer,
    int screenWidth, int screenHeight)
{
    // Increase the FOV and distance to spread out the view
    float fov = 300.0f;        // bigger FOV => more perspective spread
    float viewerDistance = 5.0f;

    // Access the Transform array
    TransformComponentArray* transformArray =
        (TransformComponentArray*)r3dSys->componentManager->componentArrays[COMPONENT_TRANSFORM];

    // For each entity
    for (int i = 0; i < r3dSys->base.count; i++) {
        Entity entity = r3dSys->base.entities[i];
        Transform* t = TransformComponentArray_GetData(transformArray, entity);
        if (t) {
            // Unpack transform
            Vec3 center = { t->position.x, t->position.y, t->position.z };
            float rotX = t->rotation.x;
            float rotY = t->rotation.y;
            float rotZ = t->rotation.z;
            float scale = t->scale.x; // uniform scale

            // Use that entity's color
            SDL_Color color = entityColors[entity];

            // Render the solid cube
            renderSolidCube(renderer,
                center,
                rotX, rotY, rotZ,
                scale,
                color,
                fov, viewerDistance,
                screenWidth, screenHeight);
        }
    }
}
