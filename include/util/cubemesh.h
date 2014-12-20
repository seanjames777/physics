/**
 * @file cubemesh.h
 *
 * @brief Cube mesh
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CUBEMESH_H
#define __CUBEMESH_H

#include <util/mesh.h>

namespace Physics { namespace Util {

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

}}

#endif
