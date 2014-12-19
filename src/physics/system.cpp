/**
 * @file system.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/system.h>
#include <physics/collisionshape.h>
#include <physics/body.h>
#include <physics/constraint.h>
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
      step(1.0 / 60.0),
      time(0.0),
      timeWarp(1.0),
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

// TODO: Switch to addVelocity() to avoid constantly mul/div mass
// TODO: Wrapper for addForce()

void System::resolveContact(Contact & contact) {
    float elasticity = 0.5f;
    float bias = 0.2f;

    glm::vec3 v1 = contact.b1->getVelocity();
    glm::vec3 v2 = contact.b2->getVelocity();
    float m1 = contact.b1->getInverseMass();
    float m2 = contact.b2->getInverseMass();

    glm::vec3 rvel = v1 - v2;
    float vn = glm::dot(rvel, contact.normal);

    if (contact.depth < 0.0f)
        contact.depth = 0.0f;

    float J = -(1.0f + elasticity) * vn + bias / step * contact.depth;
    J /= glm::dot(contact.normal, contact.normal) * (m1 + m2);

    // TODO: clamp acculuated value
    if (J < 0.0f)
        J = 0.0f;

    contact.b1->addImpulse( J * contact.normal);
    contact.b2->addImpulse(-J * contact.normal);

    // TODO: apply frictional impulse

    // TODO: apply angular impulse
}

void System::integrate(double t, double dt) {
    accumTime += dt * timeWarp;

    while (accumTime >= step) {
        /*gravity += glm::vec3(
            (sinf(time) + sinf(time * 0.6f) + sinf(time * 1.7) + sinf(time * 3.4f)) * 1.5f, 0, 0);*/

        for (auto body : bodies)
            body->addForce(gravity * body->getMass());

        for (auto body : bodies)
            body->integrateVelocities(step);

        std::vector<Contact> contacts; // TODO reserve and reuse

        Body *b1, *b2;
        CollisionShape *s1, *s2;

        // Using standard pointers here to avoid shared pointer overhead. This
        // is totally internal so isn't a problem for now.
        for (int i = 0; i < bodies.size(); i++) {
            b1 = bodies[i].get();

            if ((s1 = b1->getCollisionShape().get()) == nullptr)
                continue;

            for (int j = i + 1; j < bodies.size(); j++) {
                b2 = bodies[j].get();

                if ((s2 = b2->getCollisionShape().get()) == nullptr)
                    continue;

                s1->checkCollision(s2, b1, b2, contacts);
            }
        }

        for (int i = 0; i < 5; i++) {
            for (auto & contact : contacts)
                resolveContact(contact);

            //for (auto constraint : constraints)
            //    constraint->apply(time, step);
        }

        for (auto body : bodies)
            body->integrateTransform(step);

        time += step;
        accumTime -= step;
    }
}

}
