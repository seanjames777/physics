/**
 * @file body.h
 *
 * @brief Base class for all physics bodies
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __BODY_H
#define __BODY_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

namespace Physics {

class CollisionShape;

class Body {
private:

    // Linear quantities are stored as a position vector and a velocity
    // vector. At the end of the simulation step, position is integrated with
    // velocity. Angular quantities are stored as an orientation quaternion
    // and an angular velocity axis*angle vector (axis multiplied by angle).
    // Note that axis*angle quantities can be added.
    //
    // Changes to velocity and angular velocity are done either directly or
    // through an impulse. This has many useful properties, discussed elsehwere.
    // Each velocity can be modified directly, or together through the
    // addImpulse() function. Bodies also support accumulating forces and
    // torques before a simulation step, which will be applied as impulses
    // at the beginning of the step.
    //
    // Bodies store mass and inverse mass, and an inertia tensor and inverse
    // inertia tensor. Bodies may also be fixed, in which case they are
    // treated as if they have infinite mass, and do not move. TODO infinite
    // inertia tensor too? Bodies may also store a collision shape, which will
    // be used to detect and correct collisions with other collision shapes
    // attached to bodies. TODO why not required.

    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 linearVelocity;
    glm::vec3 angularVelocity; // axis * angle
    glm::vec3 force;
    glm::vec3 torque;       // axis * angle
    bool      fixed;
    float     mass;
    float     invMass;
    glm::mat3 inertiaTensor;
    glm::mat3 invInertiaTensor;
    std::shared_ptr<CollisionShape> shape;

public:

    Body(std::shared_ptr<CollisionShape> shape = nullptr);

    ~Body();

    std::shared_ptr<CollisionShape> getCollisionShape();

    void setCollisionShape(std::shared_ptr<CollisionShape> shape);

    glm::mat4 getTransform();

    void setPosition(glm::vec3 position);

    void setLinearVelocity(glm::vec3 linearVelocity);

    void setOrientation(glm::quat orientation);

    void setAngularVelocity(glm::vec3 angularVelocity);

    void setMass(float mass);

    void setInertiaTensor(glm::mat3 inertiaTensor);

    void setFixed(bool fixed);

    void addLinearVelocity(glm::vec3 velocity);

    void addAngularVelocity(glm::vec3 angularVelocity);

    void addLinearImpulse(glm::vec3 impulse);

    void addAngularImpulse(glm::vec3 impulse);

    void addImpulse(glm::vec3 impulse, glm::vec3 relPos);

    void addLinearForce(glm::vec3 force);

    void addTorque(glm::vec3 torque);

    void addForce(glm::vec3 force, glm::vec3 relPos);

    glm::vec3 getPosition();

    glm::quat getOrientation();

    glm::vec3 getLinearVelocity();

    glm::vec3 getAngularVelocity();

    glm::vec3 getVelocityAtPoint(glm::vec3 relPos);

    float getMass();

    float getInverseMass();

    glm::mat3 getInertiaTensor();

    glm::mat3 getInvInertiaTensor();

    bool getFixed();

    void integrateVelocities(double dt);

    void integrateTransform(double dt);

};

}

#endif
