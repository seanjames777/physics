/**
 * @file cubemesh.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/cubemesh.h>

namespace Physics { namespace Util {

CubeMesh::CubeMesh(std::shared_ptr<Body> body, float width, float height,
    float depth)
    : Mesh(body)
{
    MeshVertex vertices[24];
    int indices[36];

    int v0 = 0;
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;
    int idx = 0;

    // Bottom
    vertices[v0].position = glm::vec3(-width, -height, -depth);
    vertices[v1].position = glm::vec3(-width, -height,  depth);
    vertices[v2].position = glm::vec3( width, -height,  depth);
    vertices[v3].position = glm::vec3( width, -height, -depth);

    vertices[v0].normal = glm::vec3(0, -1, 0);
    vertices[v1].normal = glm::vec3(0, -1, 0);
    vertices[v2].normal = glm::vec3(0, -1, 0);
    vertices[v3].normal = glm::vec3(0, -1, 0);

    vertices[v0].uv = glm::vec2(0, 0);
    vertices[v1].uv = glm::vec2(0, 1);
    vertices[v2].uv = glm::vec2(1, 1);
    vertices[v3].uv = glm::vec2(1, 0);

    indices[idx++] = v0;
    indices[idx++] = v1;
    indices[idx++] = v2;
    indices[idx++] = v0;
    indices[idx++] = v2;
    indices[idx++] = v3;

    v0 += 4;
    v1 += 4;
    v2 += 4;
    v3 += 4;

    // Top
    vertices[v0].position = glm::vec3(-width, height, -depth);
    vertices[v1].position = glm::vec3(-width, height,  depth);
    vertices[v2].position = glm::vec3( width, height,  depth);
    vertices[v3].position = glm::vec3( width, height, -depth);

    vertices[v0].normal = glm::vec3(0, 1, 0);
    vertices[v1].normal = glm::vec3(0, 1, 0);
    vertices[v2].normal = glm::vec3(0, 1, 0);
    vertices[v3].normal = glm::vec3(0, 1, 0);

    vertices[v0].uv = glm::vec2(0, 0);
    vertices[v1].uv = glm::vec2(0, 1);
    vertices[v2].uv = glm::vec2(1, 1);
    vertices[v3].uv = glm::vec2(1, 0);

    indices[idx++] = v0;
    indices[idx++] = v1;
    indices[idx++] = v2;
    indices[idx++] = v0;
    indices[idx++] = v2;
    indices[idx++] = v3;

    v0 += 4;
    v1 += 4;
    v2 += 4;
    v3 += 4;

    // Left
    vertices[v0].position = glm::vec3(-width, -height, -depth);
    vertices[v1].position = glm::vec3(-width, -height,  depth);
    vertices[v2].position = glm::vec3(-width, height,  depth);
    vertices[v3].position = glm::vec3(-width, height, -depth);

    vertices[v0].normal = glm::vec3(-1, 0, 0);
    vertices[v1].normal = glm::vec3(-1, 0, 0);
    vertices[v2].normal = glm::vec3(-1, 0, 0);
    vertices[v3].normal = glm::vec3(-1, 0, 0);

    vertices[v0].uv = glm::vec2(0, 0);
    vertices[v1].uv = glm::vec2(0, 1);
    vertices[v2].uv = glm::vec2(1, 1);
    vertices[v3].uv = glm::vec2(1, 0);

    indices[idx++] = v0;
    indices[idx++] = v1;
    indices[idx++] = v2;
    indices[idx++] = v0;
    indices[idx++] = v2;
    indices[idx++] = v3;

    v0 += 4;
    v1 += 4;
    v2 += 4;
    v3 += 4;

    // Right
    vertices[v0].position = glm::vec3(width, -height, -depth);
    vertices[v1].position = glm::vec3(width, -height,  depth);
    vertices[v2].position = glm::vec3(width, height,  depth);
    vertices[v3].position = glm::vec3(width, height, -depth);

    vertices[v0].normal = glm::vec3(1, 0, 0);
    vertices[v1].normal = glm::vec3(1, 0, 0);
    vertices[v2].normal = glm::vec3(1, 0, 0);
    vertices[v3].normal = glm::vec3(1, 0, 0);

    vertices[v0].uv = glm::vec2(0, 0);
    vertices[v1].uv = glm::vec2(0, 1);
    vertices[v2].uv = glm::vec2(1, 1);
    vertices[v3].uv = glm::vec2(1, 0);

    indices[idx++] = v0;
    indices[idx++] = v1;
    indices[idx++] = v2;
    indices[idx++] = v0;
    indices[idx++] = v2;
    indices[idx++] = v3;

    v0 += 4;
    v1 += 4;
    v2 += 4;
    v3 += 4;

    // Back
    vertices[v0].position = glm::vec3(-width, -height, -depth);
    vertices[v1].position = glm::vec3(-width, height, -depth);
    vertices[v2].position = glm::vec3( width, height, -depth);
    vertices[v3].position = glm::vec3( width, -height, -depth);

    vertices[v0].normal = glm::vec3(0, 0, -1);
    vertices[v1].normal = glm::vec3(0, 0, -1);
    vertices[v2].normal = glm::vec3(0, 0, -1);
    vertices[v3].normal = glm::vec3(0, 0, -1);

    vertices[v0].uv = glm::vec2(0, 0);
    vertices[v1].uv = glm::vec2(0, 1);
    vertices[v2].uv = glm::vec2(1, 1);
    vertices[v3].uv = glm::vec2(1, 0);

    indices[idx++] = v0;
    indices[idx++] = v1;
    indices[idx++] = v2;
    indices[idx++] = v0;
    indices[idx++] = v2;
    indices[idx++] = v3;

    v0 += 4;
    v1 += 4;
    v2 += 4;
    v3 += 4;

    // Front
    vertices[v0].position = glm::vec3(-width, -height, depth);
    vertices[v1].position = glm::vec3(-width, height, depth);
    vertices[v2].position = glm::vec3( width, height, depth);
    vertices[v3].position = glm::vec3( width, -height, depth);

    vertices[v0].normal = glm::vec3(0, 0, 1);
    vertices[v1].normal = glm::vec3(0, 0, 1);
    vertices[v2].normal = glm::vec3(0, 0, 1);
    vertices[v3].normal = glm::vec3(0, 0, 1);

    vertices[v0].uv = glm::vec2(0, 0);
    vertices[v1].uv = glm::vec2(0, 1);
    vertices[v2].uv = glm::vec2(1, 1);
    vertices[v3].uv = glm::vec2(1, 0);

    indices[idx++] = v0;
    indices[idx++] = v1;
    indices[idx++] = v2;
    indices[idx++] = v0;
    indices[idx++] = v2;
    indices[idx++] = v3;

    v0 += 4;
    v1 += 4;
    v2 += 4;
    v3 += 4;

    setVertices(vertices, 24);
    setIndices(indices, 36);
}

}}
