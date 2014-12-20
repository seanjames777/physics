/**
 * @file collisionshape.h
 *
 * @brief Shapes which can be checked for collisions
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __COLLISIONSHAPE_H
#define __COLLISIONSHAPE_H

#include <glm/glm.hpp>
#include <vector>

namespace Physics {

class Body;

struct Contact {
    glm::vec3  position;
    glm::vec3  normal;
    float      depth;
    Body      *b1;
    Body      *b2;
};

class CollisionShape {
public:

    CollisionShape();

    virtual ~CollisionShape() = 0;

    virtual void checkCollision(CollisionShape *other, Body *b1, Body *b2,
        std::vector<Contact> & contacts) = 0;

    virtual void getBoundingBox(Body *body, glm::vec3 & min, glm::vec3 & max) = 0;

};

}

#endif
