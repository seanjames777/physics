/**
 * @file spheremesh.h
 *
 * @brief Sphere mesh
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SPHEREMESH_H
#define __SPHEREMESH_H

#include <util/graphics/mesh.h>

namespace Physics { namespace Util {

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

}}

#endif
