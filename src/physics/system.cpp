/**
 * @file system.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/system.h>
#include <physics/collision/shape.h>
#include <physics/dynamics/body.h>
#include <physics/constraints/constraint.h>
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
      timeWarp(0.5),
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
// TODO: Test rest on ramp

void System::resolveContact(Contact & contact) {
    // Bodies may be penetrating. We want to apply an impulse which will cause
    // them to separate. We also want to apply impulses which will eliminate
    // relative velocity tangent to the collision normal, simulating friction.
    // We run this process several times over all constraints, hoping they
    // approach a stable set of impulses. We need to separate objects and
    // apply friction but also keep stacks stable.

    float elasticity = 0.4f; // "Bounciness" or coefficient of restitution
    float bias = 0.3f;       // Additional impulse along normal for separation

    // Position of contact relative to centers of mass
    glm::vec3 r1 = contact.position - contact.b1->getPosition();
    glm::vec3 r2 = contact.position - contact.b2->getPosition();

    // Inverse mass
    float m1 = contact.b1->getInverseMass();
    float m2 = contact.b2->getInverseMass();

    // Relative velocities at relative positions
    glm::vec3 v1 = contact.b1->getVelocityAtPoint(r1);
    glm::vec3 v2 = contact.b2->getVelocityAtPoint(r2);

    // Relative velocity along normal
    glm::vec3 rvel = v1 - v2;
    float vn = glm::dot(rvel, contact.normal);

    glm::vec3 t1 = glm::cross(contact.normal, rvel);
    glm::vec3 t2 = glm::cross(contact.normal, t1);

    if (contact.depth < 0.0f) contact.depth = 0.0f; // TODO

    // Normal constraint
    {
        float J = -(1.0f + elasticity) * vn + bias / step * contact.depth;
        float div = m1 + m2;
        /*div += glm::dot(
            (contact.b1->getInvInertiaTensor() * glm::cross(glm::cross(r1, contact.normal), r1) +
             contact.b2->getInvInertiaTensor() * glm::cross(glm::cross(r2, contact.normal), r2)),
            contact.normal);*/
        J /= div;

        // TODO: clamp acculuated value. TODO discuss.
        if (J < 0.0f)
            J = 0.0f;

        contact.b1->addImpulse( J * contact.normal, r1);
        contact.b2->addImpulse(-J * contact.normal, r2);
    }

    /*float vt1 = glm::dot(rvel, t1);

    // Tangent/friction constraint 1
    {
        float J = -vt1;
        float div = m1 + m2;
        div += glm::dot(
            (contact.b1->getInvInertiaTensor() * glm::cross(glm::cross(r1, t1), r1) +
             contact.b2->getInvInertiaTensor() * glm::cross(glm::cross(r2, t1), r2)),
            t1);
        J /= div;

        // TODO: clamp acculuated value
        //if (J < 0.0f)
        //    J = 0.0f;

        contact.b1->addImpulse( J * t1, r1);
        contact.b2->addImpulse(-J * t1, r2);
    }

    float vt2 = glm::dot(rvel, t2);

    // Tangent/friction constraint 2
    {
        float J = -vt2;
        float div = m1 + m2;
        div += glm::dot(
            (contact.b1->getInvInertiaTensor() * glm::cross(glm::cross(r1, t2), r1) +
             contact.b2->getInvInertiaTensor() * glm::cross(glm::cross(r2, t2), r2)),
            t2);
        J /= div;

        // TODO: clamp acculuated value
        //if (J < 0.0f)
        //    J = 0.0f;

        contact.b1->addImpulse( J * t2, r1);
        contact.b2->addImpulse(-J * t2, r2);
    }*/
}

void System::integrate(double t, double dt) {
    accumTime += dt * timeWarp;

    while (accumTime >= step) {
        contacts.clear();
        /*gravity += glm::vec3(
            (sinf(time) + sinf(time * 0.6f) + sinf(time * 1.7) + sinf(time * 3.4f)) * 1.5f, 0, 0);*/

        for (auto body : bodies)
            body->addLinearForce(gravity * body->getMass());

        for (auto body : bodies)
            body->integrateVelocities(step);

        // TODO: check if vector re-allocates every time

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

        for (int i = 0; i < 10; i++) {
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

std::vector<Contact> & System::getContacts() {
    return contacts;
}

std::vector<std::shared_ptr<Body>> & System::getBodies() {
    return bodies;
}

}
