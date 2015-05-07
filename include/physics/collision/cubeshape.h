/**
 * @file cubeshape.h
 *
 * @brief Box/cube collision shape
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CUBESHAPE_H
#define __CUBESHAPE_H

#include <physics/collision/shape.h>

namespace Physics {

class PHYSICS_EXPORT CubeShape : public Shape {
private:

    float width;
    float height;
    float depth;

public:

    CubeShape(float width, float height, float depth);

    ~CubeShape();

    float getWidth() const;

    float getHeight() const;

    float getDepth() const;

};

}

#endif
