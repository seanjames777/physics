/**
 * @file sphereshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/collision/sphereshape.h>

namespace Physics {

SphereShape::SphereShape(float radius)
    : Shape(Shape::Sphere),
      r(radius)
{
}

SphereShape::~SphereShape() {
}

float SphereShape::getRadius() const {
    return r;
}

/*void SphereShape::getBoundingBox(Transform & t, glm::vec3 & min, glm::vec3 & max) {
    glm::vec3 position = t.position;
    glm::vec3 rad = glm::vec3(r, r, r);

    min = position - rad;
    max = position + rad;
}*/

}
