/**
 * @file constraint.h
 *
 * @brief Base class for all constraints, which add forces between objects not
 * due to collisions.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CONSTRAINT_H
#define __CONSTRAINT_H

#include <physics/defs.h>

namespace Physics {

class PHYSICS_EXPORT Constraint {
public:

    Constraint();

    virtual ~Constraint() = 0;

    virtual void apply(double t, double dt) = 0;

};

}

#endif
