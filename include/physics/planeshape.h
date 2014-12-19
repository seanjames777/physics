/**
 * @file planeshape.h
 *
 * @brief Plane collision shape
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __PLANESHAPE_H
#define __PLANESHAPE_H

#include <physics/collisionshape.h>

namespace Physics {

class PlaneShape : public CollisionShape {
private:

    glm::vec3 normal;
    float dist;

public:

    PlaneShape(glm::vec3 normal, float dist);

    ~PlaneShape();

    glm::vec3 getNormal();

    float getDistance();

    virtual void checkCollision(CollisionShape *other, Body *b1, Body *b2,
        std::vector<Contact> & contacts) override;
};

}

#endif
