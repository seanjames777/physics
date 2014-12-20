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

    float getMass();

    float getInverseMass();

    bool getFixed();

    void integrateVelocities(double dt);

    void integrateTransform(double dt);

};

}

#endif
