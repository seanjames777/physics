/**
 * @file transform.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/transform.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Physics {

glm::mat4 Transform::getLocalToWorld() {
    return glm::translate(glm::mat4(), position) * glm::mat4_cast(orientation);
}

}
