/**
 * @file springconstraint.h
 *
 * @brief Spring constraint between two bodies
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SPRINGCONSTRAINT_H
#define __SPRINGCONSTRAINT_H

#include <physics/constraint.h>
#include <physics/body.h>

namespace Physics {

class SpringConstraint : public Constraint {
private:

    std::shared_ptr<Body> b1;
    std::shared_ptr<Body> b2;
    float k;
    float length;
    float damping;

public:

    SpringConstraint(std::shared_ptr<Body> b1, std::shared_ptr<Body> b2,
        float k, float length, float damping);

    ~SpringConstraint();

    void apply(double t, double dt);

};

}

#endif
