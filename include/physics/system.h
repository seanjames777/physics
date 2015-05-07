/**
 * @file system.h
 *
 * @brief Physics system and integrator
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <glm/vec3.hpp>
#include <vector>
#include <memory>
#include <physics/collision/collision.h>

namespace Physics {

class Body;
class Constraint;

// TODO: Using shared pointer everywhere might hurt perf

/**
 * @brief Physics system, which tracks bodies that may interact, and integrates
 * their motion and forces.
 */
class PHYSICS_EXPORT System {
public:

    // TODO ugly
    struct ContactEx {
        Collision::Contact  contact;
        Body               *b1;
        Body               *b2;
    };

private:

    std::vector<std::shared_ptr<Body>> bodies;
    std::vector<std::shared_ptr<Constraint>> constraints;
    glm::vec3 gravity;
    double step;
    double accumTime;
    double time;
    double timeWarp; // TODO doubles are too big maybe
    std::vector<ContactEx> contacts;

    void resolveContact(const ContactEx & contact);

public:

    System();

    ~System();

    void addBody(std::shared_ptr<Body> body);

    void addConstraint(std::shared_ptr<Constraint> constraint);

    double getTimeWarp();

    void setTimeWarp(double timeWarp);

    // TODO: in seconds
    void integrate(double t, double dt);

    glm::vec3 getGravity();

    void setGravity(glm::vec3 gravity);

    std::vector<ContactEx> & getContacts(); // TODO

    std::vector<std::shared_ptr<Body>> & getBodies();

};

}

#endif
