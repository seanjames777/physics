/**
 * @file sphereshape.h
 *
 * @brief Sphere collision shape
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SPHERESHAPE_H
#define __SPHERESHAPE_H

#include <physics/collisionshape.h>

namespace Physics {

class SphereShape : public CollisionShape {
private:

    float r;

public:

    SphereShape(float radius);

    ~SphereShape();

    float getRadius();

    virtual void checkCollision(CollisionShape *other, Body *b1, Body *b2,
        std::vector<Contact> & contacts) override;

    virtual void getBoundingBox(Body *body, glm::vec3 & min, glm::vec3 & max) override;
};

}

#endif
