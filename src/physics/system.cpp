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

// TODO: constraints that don't require extra bodies

namespace Physics {

System::System()
    : gravity(glm::vec3(0, -9.8f, 0)),
      step(1.0 / 60.0),
      time(0.0),
      timeWarp(1.0),
      accumTime(0.0)
{
    Collision::initialize();
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

double System::getTimeWarp() {
    return timeWarp;
}

void System::setTimeWarp(double timeWarp) {
    this->timeWarp = timeWarp;
}

// TODO: Switch to addVelocity() to avoid constantly mul/div mass
// TODO: Wrapper for addForce()
// TODO: Test rest on ramp

void System::resolveContact(const ContactEx & contact) {
    // Bodies may be penetrating. We want to apply an impulse which will cause
    // them to separate. We also want to apply impulses which will eliminate
    // relative velocity tangent to the collision normal, simulating friction.
    // We run this process several times over all constraints, hoping they
    // approach a stable set of impulses. We need to separate objects and
    // apply friction but also keep stacks stable.

    float elasticity = 0.1f; // "Bounciness" or coefficient of restitution
    float bias = 0.3f;       // Additional impulse along normal for separation

    // Position of contact relative to centers of mass
    glm::vec3 p = contact.contact.position;
    glm::vec3 r1 = p - contact.b1->getPosition();
    glm::vec3 r2 = p - contact.b2->getPosition();

    // Inverse mass
    float im1 = contact.b1->getInverseMass();
    float im2 = contact.b2->getInverseMass();
    glm::mat3 iI1 = contact.b1->getInvInertiaTensor();
    glm::mat3 iI2 = contact.b2->getInvInertiaTensor();

    // Relative velocity along normal
    // Normal points from 1 -> 2
    glm::vec3 rvel = contact.b2->getVelocityAtPoint(r2) - contact.b1->getVelocityAtPoint(r1);
    glm::vec3 n = contact.contact.normal;
    float vn = glm::dot(rvel, n);

    //if (contact.contact.depth < 0.0f) contact.contact.depth = 0.0f; // TODO

    float Jn = 0.0f;

    // Relative velocity along normal should be zero
    {
        Jn = -(1.0f + elasticity) * vn + bias / step * contact.contact.depth;
        float div = im1 + im2;
        div += glm::dot(
            (iI1 * glm::cross(glm::cross(r1, n), r1) +
             iI2 * glm::cross(glm::cross(r2, n), r2)),
            n);
        Jn /= div;

        // TODO: clamp acculuated value. TODO discuss.
        if (Jn < 0.0f)
            Jn = 0.0f;

        contact.b1->addImpulse(-Jn * n, r1);
        contact.b2->addImpulse( Jn * n, r2);
    }

    if (glm::length(rvel) == 0.0f)
        return;

    glm::vec3 t = rvel - n * vn;
    float tl = glm::length(t);

    if (tl == 0.0f)
        return;

    t /= tl;

    const float fric_clamp = 0.4f * Jn;

    float vt = glm::dot(rvel, t);

    // Relative velocity off of normal should be zero
    {
        float J = -vt;
        float div = im1 + im2;
        div += glm::dot(
            (iI1 * glm::cross(glm::cross(r1, t), r1) +
             iI2 * glm::cross(glm::cross(r2, t), r2)),
            t);
        J /= div;

        if (J < -fric_clamp)
            J = -fric_clamp;
        else if (J > fric_clamp)
            J = fric_clamp;

        contact.b1->addImpulse(-J * t, r1);
        contact.b2->addImpulse( J * t, r2);
    }
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
        Shape *s1, *s2;

        // Using standard pointers here to avoid shared pointer overhead. This
        // is totally internal so isn't a problem for now.
        for (int i = 0; i < bodies.size(); i++) {
            b1 = bodies[i].get();

            if ((s1 = b1->getShape().get()) == nullptr)
                continue;

            for (int j = i + 1; j < bodies.size(); j++) {
                b2 = bodies[j].get();

                if ((s2 = b2->getShape().get()) == nullptr)
                    continue;

                ContactEx contact;
                contact.b1 = b1;
                contact.b2 = b2;

                if (Collision::checkCollision(*s1, *s2, b1->getTransform(),
                    b2->getTransform(), contact.contact))
                    contacts.push_back(contact);
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

std::vector<System::ContactEx> & System::getContacts() {
    return contacts;
}

std::vector<std::shared_ptr<Body>> & System::getBodies() {
    return bodies;
}

}
