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

class Body {
private:

    struct State {
        // Primary
        glm::vec3 x;
        glm::vec3 p;

        // Secondary
        glm::vec3 v;

        // Constant
        float mass;
        float invMass;

        State()
            : mass(1.0f),
              invMass(1.0f)
        {
        }

        void recalculate() {
            v = p * invMass;
        }
    };

    struct Derivative {
        glm::vec3 v;
        glm::vec3 f;
    };

    State     state;
    glm::vec3 force;
    bool      fixed;

    Derivative evaluate(
        State initial,
        double t,
        double dt,
        const Derivative & d);

public:

    Body();

    ~Body();

    glm::mat4 getTransform();

    void setPosition(glm::vec3 position);

    void setVelocity(glm::vec3 velocity);

    void setMass(float mass);

    void setFixed(bool fixed);

    void addForce(glm::vec3 force);

    glm::vec3 getPosition();

    glm::vec3 getVelocity();

    float getMass();

    bool getFixed();

    glm::vec3 getForce();

    void integrate(double t, double dt);

};

}

#endif
