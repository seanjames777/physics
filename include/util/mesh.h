/**
 * @file mesh.h
 *
 * @brief Mesh rendering utility
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __MESH_H
#define __MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <util/shader.h>
#include <physics/body.h>

namespace Physics { namespace Util {

/**
 * @brief Mesh vertex type
 */
struct MeshVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

/**
 * @brief Mesh rendering utility
 */
class Mesh {
private:

    GLuint                  vb;
    GLuint                  ib;
    GLuint                  vao;
    std::shared_ptr<Body>   body;
    int                     nIndices;

public:

    Mesh(std::shared_ptr<Body> body);

    virtual ~Mesh();

    glm::mat4 getTransform();

    void setVertices(MeshVertex *vertices, int nVertices);

    void setIndices(int *indices, int nIndices);

    void draw();

};

}}

#endif
