/**
 * @file system.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/system.h>
#include <iostream>

// Physics notes:
//
//   x          (position)
//   m          (mass)
//   v = dx/dt  (velocity)
//   a = dv/dt  (acceleration)
//   F = m * a  (force)
//   p = mv     (momentum)
//   F = dp/dt
//

namespace Physics {

System::System()
    : gravity(glm::vec3(0, -9.8f, 0)),
      step(1.0 / 250.0),
      time(0.0),
      accumTime(0.0)
{
}

System::~System() {
}

glm::vec3 System::getGravity() {
    return gravity;
}

void System::setGravity(glm::vec3 gravity) {
    this->gravity = gravity;
}

void System::addBody(std::shared_ptr<Body> body) {
    bodies.push_back(body);
}

void System::addConstraint(std::shared_ptr<Constraint> constraint) {
    constraints.push_back(constraint);
}

void System::integrate(double t, double dt) {
    accumTime += dt;

    while (accumTime >= step) {
        gravity = glm::vec3(0, -9.8f, 0);
        /*gravity += glm::vec3(
            (sinf(time) + sinf(time * 0.6f) + sinf(time * 1.7) + sinf(time * 3.4f)) * 1.5f, 0, 0);*/

        for (auto body : bodies)
            body->addForce(gravity * body->getMass());

        for (auto constraint : constraints)
            constraint->apply(time, step);

        for (auto body : bodies)
            body->integrate(time, step);

        time += step;
        accumTime -= step;
    }
}

}
