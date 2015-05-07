#include <util/mesh.h>

Mesh::Mesh(BufferUsage usage)
    : vertexBuffer(std::make_shared<Buffer>(BufferBindingVertexBuffer, usage)),
      indexBuffer(std::make_shared<Buffer>(BufferBindingIndexBuffer, usage)),
      uniformBuffer(std::make_shared<Buffer>(BufferBindingUniformBuffer, BufferUsageStream)),
      mode(MeshRenderModeTriangles),
      numIndices(0)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vertexBuffer->bind();
    indexBuffer->bind();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 48, (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 48, (void *)12);
    glVertexAttribPointer(2, 4, GL_FLOAT, false, 48, (void *)24);
    glVertexAttribPointer(3, 2, GL_FLOAT, false, 48, (void *)40);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
}

void Mesh::draw(int uniformsIdx) {
    uniformBuffer->bind(uniformsIdx);
    glBindVertexArray(vao);

    glDrawElements((GLenum)mode, numIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    uniformBuffer->unbind(uniformsIdx);
}

void Mesh::setVertices(MeshVertex *vertices, size_t numVertices) {
    vertexBuffer->bind();
    vertexBuffer->setData(numVertices * sizeof(MeshVertex), vertices);
    vertexBuffer->unbind();
}

void Mesh::setIndices(unsigned int *indices, size_t numIndices) {
    indexBuffer->bind();
    indexBuffer->setData(numIndices * sizeof(unsigned int), indices);
    indexBuffer->unbind();
    this->numIndices = numIndices;
}

void Mesh::flushUniforms() {
    uniformBuffer->setData(sizeof(MeshUniforms), &uniforms);
}

void Mesh::setRenderMode(MeshRenderMode mode) {
    this->mode = mode;
}

MeshUniforms *Mesh::getUniforms() {
    return &uniforms;
}
