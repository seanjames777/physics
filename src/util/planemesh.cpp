/**
 * @file planemesh.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/planemesh.h>

namespace Physics { namespace Util {

PlaneMesh::PlaneMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Body> body)
    : Mesh(shader, body)
{
    MeshVertex vertices[4];
    int indices[6];

    vertices[0].position = glm::vec3(-1, 0, -1);
    vertices[1].position = glm::vec3(-1, 0,  1);
    vertices[2].position = glm::vec3( 1, 0,  1);
    vertices[3].position = glm::vec3( 1, 0, -1);

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

    setVertices(vertices, 4);
    setIndices(indices, 6);
}

}}
