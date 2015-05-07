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
#include <glm/glm.hpp>

namespace Physics {

class PHYSICS_EXPORT PlaneShape : public Shape {
private:

    glm::vec3 normal;
    float dist;

public:

    PlaneShape(glm::vec3 normal, float dist);

    ~PlaneShape();

    glm::vec3 getNormal() const;

    float getDistance() const;

};

}

#endif
