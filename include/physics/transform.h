/**
 * @file transform.h
 *
 * @brief Representation of position and orientation transform
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#include <physics/defs.h>

namespace Physics {

struct PHYSICS_EXPORT Transform {
    glm::vec3 position;    //!< Position vector
    glm::quat orientation; //!< Orientation quaternion

    /**
     * @brief Get a transformation matrix
     */
    glm::mat4 getLocalToWorld() const;

    /**
     * @brief Transform a point
     */
    void transform(glm::vec3 & point) const;
};

}

#endif
