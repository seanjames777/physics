/**
 * @file spheremesh.h
 *
 * @brief Sphere mesh
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SPHEREMESH_H
#define __SPHEREMESH_H

#include <graphics/mesh.h>

namespace Graphics {

/**
 * @brief Sphere mesh
 */
class SphereMesh : public Mesh {
public:

    /**
     * @brief Constructor
     *
     * @param[in] shader Shader
     */
    SphereMesh(int divu, int divv, float r);

};

}

#endif
