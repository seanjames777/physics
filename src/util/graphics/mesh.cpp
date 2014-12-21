/**
 * @file mesh.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/graphics/mesh.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Physics { namespace Util {

Mesh::Mesh()
    : vb(0),
      ib(0),
      vao(0),
      nIndices(0),
      mode(GL_TRIANGLES)
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

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), offset);
    offset += sizeof(float) * 4;

    glBindVertexArray(0);
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

void Mesh::setMode(GLenum mode) {
    this->mode = mode;
}

void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(mode, nIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0); // TODO
}

}}
