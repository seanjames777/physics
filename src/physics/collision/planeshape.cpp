/**
 * @file planeshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/collision/planeshape.h>

namespace Physics {

PlaneShape::PlaneShape(glm::vec3 normal, float dist)
    : Shape(Shape::Plane),
      normal(normal),
      dist(dist)
{
}

PlaneShape::~PlaneShape() {
}

glm::vec3 PlaneShape::getNormal() const {
    return normal;
}

float PlaneShape::getDistance() const {
    return dist;
}

/*void PlaneShape::getBoundingBox(Transform & t, glm::vec3 & min, glm::vec3 & max) {
    min = glm::vec3(-1.0 / 0.0, 0.0f, -1.0 / 0.0); // TODO
    min = glm::vec3( 1.0 / 0.0, 0.0f,  1.0 / 0.0);
}*/

}
