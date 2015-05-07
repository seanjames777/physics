/**
 * @file sphereshape.h
 *
 * @brief Sphere collision shape
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SPHERESHAPE_H
#define __SPHERESHAPE_H

#include <physics/collision/shape.h>

namespace Physics {

class PHYSICS_EXPORT SphereShape : public Shape {
private:

    float r;

public:

    SphereShape(float radius);

    ~SphereShape();

    float getRadius() const;

};

}

#endif
