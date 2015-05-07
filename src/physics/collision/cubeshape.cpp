/**
 * @file cubeshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/collision/cubeshape.h>

namespace Physics {

CubeShape::CubeShape(float width, float height, float depth)
    : Shape(Shape::Cube),
      width(width),
      height(height),
      depth(depth)
{
}

CubeShape::~CubeShape() {
}

float CubeShape::getWidth() const {
    return width;
}

float CubeShape::getHeight() const {
    return height;
}

float CubeShape::getDepth() const {
    return depth;
}

/*void CubeShape::getBoundingBox(Transform & t, glm::vec3 & min, glm::vec3 & max) {
    glm::vec3 position = t.position;
    glm::vec3 rad = glm::vec3(r, r, r);

    min = position - rad;
    max = position + rad;
}*/

}
