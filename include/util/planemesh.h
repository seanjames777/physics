/**
 * @file planemesh.h
 *
 * @brief Plane mesh
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __PLANEMESH_H
#define __PLANEMESH_H

#include <util/mesh.h>

namespace Physics { namespace Util {

/**
 * @brief Plane mesh
 */
class PlaneMesh : public Mesh {
public:

    /**
     * @brief Constructor
     *
     * @param[in] shader Shader
     */
    PlaneMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Body> body,
        float width, float height);

};

}}

#endif
