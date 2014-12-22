/**
 * @file cubemesh.h
 *
 * @brief Cube mesh
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CUBEMESH_H
#define __CUBEMESH_H

#include <graphics/mesh.h>

namespace Graphics {

/**
 * @brief Cube mesh
 */
class CubeMesh : public Mesh {
public:

    /**
     * @brief Constructor
     *
     * @param[in] shader Shader
     */
    CubeMesh(float width, float height, float depth);

};

}

#endif
