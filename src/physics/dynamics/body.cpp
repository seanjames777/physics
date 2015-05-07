/**
 * @file body.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/dynamics/body.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream> // TODO

namespace Physics {

Body::Body(std::shared_ptr<Shape> shape)
    : fixed(false),
      mass(1.0f),
      invMass(1.0f),
      shape(shape)
{
}

Body::~Body() {
}

std::shared_ptr<Shape> Body::getShape() {
    return shape;
}

void Body::setShape(std::shared_ptr<Shape> shape) {
    this->shape = shape;
}

glm::vec3 Body::getPosition() {
    return transform.position;
}

glm::vec3 Body::getLinearVelocity() {
    return linearVelocity;
}

Transform & Body::getTransform() {
    return transform;
}

glm::quat Body::getOrientation() {
    return transform.orientation;
}

glm::vec3 Body::getAngularVelocity() {
    return angularVelocity;
}

glm::vec3 Body::getVelocityAtPoint(glm::vec3 relPos) {
    return linearVelocity + glm::cross(angularVelocity, relPos);
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

glm::mat3 Body::getInertiaTensor() {
    if (fixed)
        return glm::mat3(1.0f / 0.0f);

    return inertiaTensor;
}

glm::mat3 Body::getInvInertiaTensor() {
    if (fixed)
        return glm::mat3(0.0f);

    return invInertiaTensor;
}

bool Body::getFixed() {
    return fixed;
}

glm::mat4 Body::getLocalToWorld() {
    return transform.getLocalToWorld();
}

void Body::setPosition(glm::vec3 position) {
    transform.position = position;
}

void Body::setOrientation(glm::quat orientation) {
    transform.orientation = orientation;
}

void Body::setLinearVelocity(glm::vec3 velocity) {
    this->linearVelocity = velocity;
}

void Body::setAngularVelocity(glm::vec3 velocity) {
    this->angularVelocity = velocity;
}

void Body::setMass(float mass) {
    this->mass = mass;
    this->invMass = 1.0f / mass;
}

void Body::setInertiaTensor(glm::mat3 inertiaTensor) {
    this->inertiaTensor = inertiaTensor;
    this->invInertiaTensor = glm::inverse(inertiaTensor); // TODO diagonal
}

void Body::setFixed(bool fixed) {
    this->fixed = fixed;
}

void Body::addLinearVelocity(glm::vec3 velocity) {
    // TODO Inline these functions
    // TODO Fixed makes a branch
    if (fixed)
        return;

    this->linearVelocity += velocity;
}

void Body::addAngularVelocity(glm::vec3 velocity) {
    if (fixed)
        return;

    this->angularVelocity += velocity;
}

void Body::addLinearImpulse(glm::vec3 impulse) {
    addLinearVelocity(invMass * impulse);
}

void Body::addAngularImpulse(glm::vec3 impulse) {
    addAngularVelocity(invInertiaTensor * impulse);
}

void Body::addImpulse(glm::vec3 impulse, glm::vec3 relPos) {
    addLinearImpulse(impulse);
    addAngularImpulse(glm::cross(relPos, impulse));
}

void Body::addLinearForce(glm::vec3 force) {
    this->force += force;
}

void Body::addTorque(glm::vec3 torque) {
    this->torque += torque;
}

// TODO might want to pass vectors by reference all over. Probably the
// compiler is not stupid though
// TODO const things maybe
void Body::addForce(glm::vec3 force, glm::vec3 relPos) {
    addLinearForce(force);
    addTorque(glm::cross(relPos, force));
}

void Body::integrateVelocities(double dt) {
    addLinearVelocity(invMass * force * (float)dt);
    addAngularVelocity(invInertiaTensor * torque * (float)dt);

    // TODO damping factor
    //linearVelocity *= 1.0f - 0.1f * dt;
    //angularVelocity *= 1.0f - 0.1f * dt;

    force = glm::vec3();
    torque = glm::vec3();
}

void Body::integrateTransform(double dt) {
    transform.position += linearVelocity * (float)dt;

    glm::vec3 scaledAngularVelocity = angularVelocity;

    float angle = glm::length(scaledAngularVelocity);

    if (angle != 0.0f) {
        glm::vec3 axis = scaledAngularVelocity / angle;

        // TODO GLM is in degrees...
        transform.orientation = glm::rotate(transform.orientation,
            angle * 180.0f / (float)M_PI * (float)dt, axis);
    }
}

}
