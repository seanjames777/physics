/**
 * @file rodconstraint.h
 *
 * @brief Rod (fixed distance) constraint between two bodies
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __RODCONSTRAINT_H
#define __RODCONSTRAINT_H

#include <physics/constraint.h>
#include <physics/body.h>

namespace Physics {

class RodConstraint : public Constraint {
private:

    std::shared_ptr<Body> b1;
    std::shared_ptr<Body> b2;
    float length;

public:

    RodConstraint(std::shared_ptr<Body> b1, std::shared_ptr<Body> b2,
        float length);

    ~RodConstraint();

    void apply(double t, double dt);

};

}

#endif
