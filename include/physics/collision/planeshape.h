/**
 * @file planeshape.h
 *
 * @brief Plane collision shape
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __PLANESHAPE_H
#define __PLANESHAPE_H

#include <physics/collision/shape.h>

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

    virtual void getBoundingBox(Body *body, glm::vec3 & min, glm::vec3 & max) override;
};

}

#endif
