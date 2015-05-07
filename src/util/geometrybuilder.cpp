#include <util/geometrybuilder.h>
#include <vector>

std::shared_ptr<Mesh> GeometryBuilder::createPlane(
    float width,
    float height)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(BufferUsageStatic);

    MeshVertex vertices[4];
    unsigned int indices[6];

    vertices[0].position = glm::vec3(-width, 0, -height);
    vertices[1].position = glm::vec3(-width, 0,  height);
    vertices[2].position = glm::vec3( width, 0,  height);
    vertices[3].position = glm::vec3( width, 0, -height);

    vertices[0].normal = glm::vec3(0, 1, 0);
    vertices[1].normal = glm::vec3(0, 1, 0);
    vertices[2].normal = glm::vec3(0, 1, 0);
    vertices[3].normal = glm::vec3(0, 1, 0);

    vertices[0].uv = glm::vec2(0, 0);
    vertices[1].uv = glm::vec2(0, 1);
    vertices[2].uv = glm::vec2(1, 1);
    vertices[3].uv = glm::vec2(1, 0);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;

    mesh->setVertices(&vertices[0], 4);
    mesh->setIndices(&indices[0], 6);

    return mesh;
}

std::shared_ptr<Mesh> GeometryBuilder::createCube(
    float width,
    float height,
    float depth)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(BufferUsageStatic);

    MeshVertex vertices[24];
    unsigned int indices[36];

    unsigned int v0 = 0;
    unsigned int v1 = 1;
    unsigned int v2 = 2;
    unsigned int v3 = 3;
    unsigned int idx = 0;

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

    mesh->setVertices(&vertices[0], 24);
    mesh->setIndices(&indices[0], 36);

    return mesh;
}

inline glm::vec3 spherePos(float u, float v, float r) {
    float theta = u * 2.0f * M_PI;
    float phi = v * M_PI;

    return glm::vec3(
        sinf(phi) * cosf(theta),
        cosf(phi),
        sinf(phi) * sinf(theta)) * r;
}

std::shared_ptr<Mesh> GeometryBuilder::createSphere(
    int nu,
    int nv,
    float r)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(BufferUsageStatic);

    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;

    MeshVertex vertex;
    vertex.position = glm::vec3(0, r, 0);
    vertex.normal = glm::vec3(0, 1, 0);
    vertex.uv = glm::vec2(0, 0);
    vertices.push_back(vertex);

    for (int y = 0; y < nv; y++) {
        float v = (float)(y + 1) / (float)(nv + 1);

        for (int x = 0; x <= nu; x++) {
            float u = (float)x / (float)nu;

            vertex.position = spherePos(u, v, r);
            vertex.normal = glm::normalize(vertex.position);
            vertex.uv = glm::vec2(u, v);

            vertices.push_back(vertex);
        }
    }

    vertex.position = glm::vec3(0, -r, 0);
    vertex.normal = glm::vec3(0, -1, 0);
    vertex.uv = glm::vec2(1, 1);
    vertices.push_back(vertex);

    for (int x = 0; x < nu; x++) {
        indices.push_back(0);
        indices.push_back(x + 1);
        indices.push_back(x + 2);
    }

    for (int y = 0; y < nv - 1; y++) {
        int y0 = y * (nu + 1) + 1;
        int y1 = (y + 1) * (nu + 1) + 1;

        for (int x = 0; x < nu; x++) {
            int i00 = y0 + x;
            int i01 = y0 + x + 1;
            int i10 = y1 + x;
            int i11 = y1 + x + 1;

            indices.push_back(i00);
            indices.push_back(i10);
            indices.push_back(i11);
            indices.push_back(i00);
            indices.push_back(i11);
            indices.push_back(i01);
        }
    }

    int i0 = (nv - 1) * (nu + 1) + 1;
    int i2 = nv * (nu + 1) + 1;

    for (int x = 0; x < nu; x++) {
        indices.push_back(i0 + x);
        indices.push_back(i0 + x + 1);
        indices.push_back(i2);
    }

    mesh->setVertices(&vertices[0], vertices.size());
    mesh->setIndices(&indices[0], indices.size());

    return mesh;
}
