/**
 * @file spheremesh.h
 *
 * @brief Sphere mesh
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SPHEREMESH_H
#define __SPHEREMESH_H

#include <util/mesh.h>

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
    SphereMesh(std::shared_ptr<Body> body, int divu, int divv, float r);

};

}}

#endif
