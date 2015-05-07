/**
 * @file transform.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/transform.h>

namespace Physics {

glm::mat4 Transform::getLocalToWorld() const {
    return glm::translate(glm::mat4(), position) * glm::mat4_cast(orientation);
}

void Transform::transform(glm::vec3 & p3) const {
    glm::vec4 p4 = glm::vec4(p3, 1.0f);
    p4 = getLocalToWorld() * p4; // TODO cache matrix
    p3 = glm::vec3(p4.x, p4.y, p4.z);
}

}
