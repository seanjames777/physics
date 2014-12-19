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
#include <memory>

namespace Physics {

class CollisionShape;

class Body {
private:

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    bool fixed;
    float     mass;
    float     invMass;
    std::shared_ptr<CollisionShape> shape;

public:

    Body(std::shared_ptr<CollisionShape> shape = nullptr);

    ~Body();

    std::shared_ptr<CollisionShape> getCollisionShape();

    void setCollisionShape(std::shared_ptr<CollisionShape> shape);

    glm::mat4 getTransform();

    void setPosition(glm::vec3 position);

    void setVelocity(glm::vec3 velocity);

    void setMass(float mass);

    void setFixed(bool fixed);

    void addVelocity(glm::vec3 velocity);

    void addImpulse(glm::vec3 impulse);

    void addForce(glm::vec3 force);

    glm::vec3 getPosition();

    glm::vec3 getVelocity();

    float getMass();

    float getInverseMass();

    bool getFixed();

    void integrateVelocities(double dt);

    void integrateTransform(double dt);

};

}

#endif
