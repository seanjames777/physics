/**
 * @file transform.h
 *
 * @brief Representation of position and orientation transform
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Physics {

struct Transform {
    glm::vec3 position;    //!< Position vector
    glm::quat orientation; //!< Orientation quaternion

    /**
     * @brief Get a transformation matrix
     */
    glm::mat4 getLocalToWorld();
};

}

#endif
