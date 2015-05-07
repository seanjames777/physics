#ifndef __MESH_H
#define __MESH_H

#include <util/gldefs.h>
#include <util/buffer.h>
#include <memory>

#pragma pack(push, 1)
struct MeshVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 color;
    glm::vec2 uv;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MeshUniforms {
    glm::mat4 world;
    glm::mat4 worldInverseTranspose;
};
#pragma pack(pop)

enum MeshRenderMode {
    MeshRenderModeTriangles = GL_TRIANGLES,
    MeshRenderModeLines     = GL_LINES
};

class Mesh {
private:

    std::shared_ptr<Buffer> vertexBuffer;
    std::shared_ptr<Buffer> indexBuffer;
    std::shared_ptr<Buffer> uniformBuffer;
    MeshRenderMode          mode;
    size_t                  numIndices;
    GLuint                  vao;
    MeshUniforms            uniforms;

public:

    Mesh(BufferUsage usage);

    ~Mesh();

    void draw(int uniformsIdx);

    void setVertices(MeshVertex *vertices, size_t numVertices);

    void setIndices(unsigned int *indices, size_t numIndices);

    void setRenderMode(MeshRenderMode mode);

    MeshUniforms *getUniforms();

    void flushUniforms();

};

#endif
