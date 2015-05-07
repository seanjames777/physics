/**
 * @file transform.h
 *
 * @brief Representation of position and orientation transform
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Physics {

struct Transform {
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
