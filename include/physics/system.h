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

namespace Physics {

class Body;
class Constraint;
class Contact;

// TODO: Using shared pointer everywhere might hurt perf

/**
 * @brief Physics system, which tracks bodies that may interact, and integrates
 * their motion and forces.
 */
class System {
private:

    std::vector<std::shared_ptr<Body>> bodies;
    std::vector<std::shared_ptr<Constraint>> constraints;
    glm::vec3 gravity;
    double step;
    double accumTime;
    double time;
    double timeWarp; // TODO doubles are too big maybe
    std::vector<Contact> contacts;

    void resolveContact(Contact & contact);

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

    std::vector<Contact> & getContacts(); // TODO

    std::vector<std::shared_ptr<Body>> & getBodies();

};

}

#endif
