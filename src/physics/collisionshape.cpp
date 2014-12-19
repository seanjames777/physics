/**
 * @file collisionshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/collisionshape.h>

namespace Physics {

CollisionShape::CollisionShape() {
}

CollisionShape::~CollisionShape() {
}

void CollisionShape::checkCollision(CollisionShape *other, Body *b1, Body *b2,
    std::vector<Contact> & contacts)
{
}

}
