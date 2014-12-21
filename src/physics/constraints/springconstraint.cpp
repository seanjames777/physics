/**
 * @file springconstraint.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/constraints/springconstraint.h>

namespace Physics {

SpringConstraint::SpringConstraint(std::shared_ptr<Body> b1,
    std::shared_ptr<Body> b2, float k, float length, float damping)
    : b1(b1),
      b2(b2),
      k(k),
      length(length),
      damping(damping)
{
}

SpringConstraint::~SpringConstraint() {
}

void SpringConstraint::apply(double t, double dt) {
    /*glm::vec3 diff = b1->getPosition() - b2->getPosition();
    float dist = glm::length(diff);
    diff = glm::normalize(diff);

    glm::vec3 force = k * diff * (dist - length);

    glm::vec3 rvel = b1->getVelocity() - b2->getVelocity();
    force += damping * rvel;

    //b1->addForce(-k * diff * (dist - length));
    //b2->addForce(force);*/
}

}
