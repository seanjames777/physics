/**
 * @file planemesh.h
 *
 * @brief Plane mesh
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __PLANEMESH_H
#define __PLANEMESH_H

#include <graphics/mesh.h>

namespace Graphics {

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
    PlaneMesh(float width, float height);

};

}

#endif
