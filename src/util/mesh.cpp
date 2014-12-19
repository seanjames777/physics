/**
 * @file mesh.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/mesh.h>

namespace Physics { namespace Util {

Mesh::Mesh(std::shared_ptr<Body> body)
    : vb(0),
      ib(0),
      vao(0),
      body(body),
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
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vb);
    glDeleteBuffers(1, &ib);
    glDeleteVertexArrays(1, &vao);
}

glm::mat4 Mesh::getTransform() {
    return body->getTransform();
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

void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0); // TODO
}

}}
