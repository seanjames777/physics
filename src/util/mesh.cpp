/**
 * @file mesh.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/mesh.h>
#include <glm/gtc/type_ptr.hpp>

namespace Physics { namespace Util {

Mesh::Mesh(std::shared_ptr<Shader> shader, std::shared_ptr<Body> body)
    : vb(0),
      ib(0),
      vao(0),
      shader(shader),
      body(body),
      worldViewProjectionLocation(0),
      nIndices(0)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vb);
    glGenBuffers(1, &ib);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

    char *offset = 0;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), offset);
    offset += sizeof(float) * 3;

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), offset);
    offset += sizeof(float) * 3;

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), offset);
    offset += sizeof(float) * 2;

    glBindVertexArray(0);

    worldInverseTransposeLocation = glGetUniformLocation(
        shader->getProgram(), "worldInverseTranspose");
    worldViewProjectionLocation = glGetUniformLocation(
        shader->getProgram(), "worldViewProjection");
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vb);
    glDeleteBuffers(1, &ib);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::setVertices(MeshVertex *vertices, int nVertices) {
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * nVertices, vertices,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setIndices(int *indices, int nIndices) {
    this->nIndices = nIndices;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * nIndices, indices,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(glm::mat4 viewProjection) {
    glUseProgram(shader->getProgram());

    glm::mat4 world = body->getTransform();
    glm::mat4 worldViewProjection = viewProjection * world;
    glm::mat4x4 worldInverseTranspose = glm::transpose(glm::inverse(world));

    glUniformMatrix4fv(worldViewProjectionLocation, 1, false,
        glm::value_ptr(worldViewProjection));
    glUniformMatrix4fv(worldInverseTransposeLocation, 1, false,
        glm::value_ptr(worldInverseTranspose));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glUseProgram(0);
}

}}
