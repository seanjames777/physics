/**
 * @file body.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/body.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Physics {

Body::Body()
    : fixed(false),
      mass(1.0f),
      invMass(1.0f)
{
}

Body::~Body() {
}

glm::vec3 Body::getPosition() {
    return state.x;
}

glm::vec3 Body::getVelocity() {
    return state.v;
}

float Body::getMass() {
    return mass;
}

bool Body::getFixed() {
    return fixed;
}

glm::vec3 Body::getForce() {
    return force;
}

glm::mat4 Body::getTransform() {
    return glm::translate(glm::mat4(), state.x);
}

void Body::setPosition(glm::vec3 position) {
    state.x = position;
}

void Body::setVelocity(glm::vec3 velocity) {
    state.v = velocity;
}

void Body::setMass(float mass) {
    mass = mass;
    invMass = 1.0f / mass;
}

void Body::setFixed(bool fixed) {
    this->fixed = fixed;
}

void Body::addForce(glm::vec3 force) {
    if (fixed)
        return;

    this->force += force;
}

// RK4 integrator
// http://gafferongames.com/game-physics/integration-basics/
Body::Derivative Body::evaluate(
    State initial,
    double t,
    double dt,
    const Body::Derivative & deriv)
{
    state.x += deriv.dx * (float)dt;
    state.v += deriv.dv * (float)dt;

    Derivative outd;
    outd.dx = state.v;
    outd.dv = force * invMass;

    return outd;
}

void Body::integrate(double t, double dt) {
    Derivative k1 = evaluate(state, t, 0.0, Derivative());
    Derivative k2 = evaluate(state, t, dt * 0.5, k1);
    Derivative k3 = evaluate(state, t, dt * 0.5, k2);
    Derivative k4 = evaluate(state, t, dt, k3);

    glm::vec3 dxdt = 1.0f / 6.0f * (k1.dx + 2.0f * (k2.dx + k3.dx) + k4.dx);
    glm::vec3 dvdt = 1.0f / 6.0f * (k1.dv + 2.0f * (k2.dv + k3.dv) + k4.dv);

    state.x += dxdt * (float)dt;
    state.v += dvdt * (float)dt;

    force = glm::vec3();
}

}
