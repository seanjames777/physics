/**
 * @file cubeshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/collision/cubeshape.h>
#include <iostream> // TODO
#include <physics/dynamics/body.h>
#include <physics/collision/planeshape.h>

namespace Physics {

CubeShape::CubeShape(float radius)
    : Shape(Shape::Cube),
      r(radius)
{
}

CubeShape::~CubeShape() {
}

float CubeShape::getRadius() {
    return r;
}

/*void CubeShape::checkCollision(Shape *other, Transform & t1, Transform & t2,
    std::vector<Contact> & contacts)
{
    Contact contact;

    // TODO: dynamic_cast forces rtti
    if (CubeShape *other_cube = dynamic_cast<CubeShape *>(other)) {
        glm::vec3 diff = t2.position - t1.position;
        // TODO: dot might be faster. also elsewhere. squared dist?
        float dist = glm::length(diff);
        glm::vec3 norm = diff / dist; // TODO dist might be zero

        if (dist < r + other_cube->r) {
            contact.normal = norm;
            contact.depth = r + other_cube->r - dist;
            contact.position = t1.position + norm * r;

            contacts.push_back(contact);
        }
    }
    else if (PlaneShape *other_plane = dynamic_cast<PlaneShape *>(other)) {
        glm::vec3 p1 = t1.position;
        float dist = glm::dot(p1, other_plane->getNormal()) - other_plane->getDistance();

        // TODO obviously move collisions into a single place
        // TODO decide on direction of normal and depth

        if (dist < r) {
            contact.normal = -other_plane->getNormal();
            contact.depth = r - dist;

            contacts.push_back(contact);
        }
    }
}*/

/*void CubeShape::getBoundingBox(Transform & t, glm::vec3 & min, glm::vec3 & max) {
    glm::vec3 position = t.position;
    glm::vec3 rad = glm::vec3(r, r, r);

    min = position - rad;
    max = position + rad;
}*/

}
