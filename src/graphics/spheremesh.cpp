/**
 * @file spheremesh.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <graphics/spheremesh.h>
#include <vector>
#include <iostream>

namespace Graphics {

glm::vec3 spherePos(float u, float v, float r) {
    float theta = u * 2.0f * M_PI;
    float phi = v * M_PI;

    return glm::vec3(
        sinf(phi) * cosf(theta),
        cosf(phi),
        sinf(phi) * sinf(theta)) * r;
}

SphereMesh::SphereMesh(int nu, int nv, float r)
{
    std::vector<MeshVertex> vertices;
    std::vector<int> indices;

    MeshVertex vertex;
    vertex.position = glm::vec3(0, r, 0);
    vertex.normal = glm::vec3(0, 1, 0);
    vertex.uv = glm::vec2(0, 0);
    vertices.push_back(vertex);

    for (int y = 0; y < nv; y++) {
        float v = (float)(y + 1) / (float)(nv + 1);

        for (int x = 0; x < nu; x++) {
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
        indices.push_back((x + 1) % nu + 1);
    }

    for (int y = 0; y < nv - 1; y++) {
        int y0 = y * nu + 1;
        int y1 = (y + 1) * nu + 1;

        for (int x = 0; x < nu; x++) {
            int i00 = y0 + x;
            int i01 = y0 + (x + 1) % nu;
            int i10 = y1 + x;
            int i11 = y1 + (x + 1) % nu;

            indices.push_back(i00);
            indices.push_back(i10);
            indices.push_back(i11);
            indices.push_back(i00);
            indices.push_back(i11);
            indices.push_back(i01);
        }
    }

    int i0 = (nv - 1) * nu + 1;
    int i2 = nv * nu + 1;

    for (int x = 0; x < nu; x++) {
        indices.push_back(i0 + x);
        indices.push_back(i0 + (x + 1) % nu);
        indices.push_back(i2);
    }

    setVertices(&vertices[0], vertices.size());
    setIndices(&indices[0], indices.size());
}

}
