/**
 * @file body.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/body.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Physics {

Body::Body(std::shared_ptr<CollisionShape> shape)
    : fixed(false),
      mass(1.0f),
      invMass(1.0f),
      shape(shape)
{
}

Body::~Body() {
}

std::shared_ptr<CollisionShape> Body::getCollisionShape() {
    return shape;
}

void Body::setCollisionShape(std::shared_ptr<CollisionShape> shape) {
    this->shape = shape;
}

glm::vec3 Body::getPosition() {
    return position;
}

glm::vec3 Body::getVelocity() {
    return velocity;
}

float Body::getMass() {
    if (fixed)
        return 1.0f / 0.0f; // Infinity // TODO

    return mass;
}

float Body::getInverseMass() {
    if (fixed)
        return 0.0f;

    return invMass;
}

bool Body::getFixed() {
    return fixed;
}

glm::mat4 Body::getTransform() {
    return glm::translate(glm::mat4(), position);
}

void Body::setPosition(glm::vec3 position) {
    this->position = position;
}

void Body::setVelocity(glm::vec3 velocity) {
    this->velocity = velocity;
}

void Body::setMass(float mass) {
    this->mass = mass;
    this->invMass = 1.0f / mass;
}

void Body::setFixed(bool fixed) {
    this->fixed = fixed;
}

void Body::addVelocity(glm::vec3 velocity) {
    // TODO Inline these functions

    if (fixed)
        return;

    this->velocity += velocity;
}

void Body::addImpulse(glm::vec3 impulse) {
    addVelocity(impulse * invMass);
}

void Body::addForce(glm::vec3 force) {
    this->force += force;
}

void Body::integrateVelocities(double dt) {
    addVelocity(force * invMass * (float)dt);
    force = glm::vec3();
}

void Body::integrateTransform(double dt) {
    position += velocity * (float)dt;
}

}
