/**
 * @file rodconstraint.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/constraints/rodconstraint.h>
#include <iostream>

namespace Physics {

RodConstraint::RodConstraint(std::shared_ptr<Body> b1,
    std::shared_ptr<Body> b2, float length)
    : b1(b1),
      b2(b2),
      length(length)
{
}

RodConstraint::~RodConstraint() {
}

void RodConstraint::apply(double t, double dt) {
    /*glm::vec3 diff = b1->getPosition() - b2->getPosition();
    float dist = glm::length(diff);
    diff = glm::normalize(diff);

    // Want to move this far, in this amount of time
    glm::vec3 move = diff * (dist - length);
    glm::vec3 vel = move * 0.15f / (float)dt;

    // Want the relative velocity along the normal to be 0
    glm::vec3 relVel = b1->getVelocity() - b2->getVelocity();
    float dot = glm::dot(diff, relVel);
    vel += diff * 0.95f * dot;

    //b1->addForce(-k * diff * (dist - length));
    //b2->addForce(force);

    b2->addImpulse(vel * b2->getMass());*/
}

}
