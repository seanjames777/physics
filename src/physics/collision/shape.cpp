/**
 * @file collisionshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/collision/shape.h>

namespace Physics {

Shape::Shape(enum Shape::ShapeType shapeType)
    : shapeType(shapeType)
{
}

Shape::~Shape() {
}

enum Shape::ShapeType Shape::getShapeType() const {
    return shapeType;
}

}
