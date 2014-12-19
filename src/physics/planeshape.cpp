/**
 * @file planeshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/planeshape.h>
#include <physics/sphereshape.h>
#include <iostream> // TODO
#include <physics/body.h>

namespace Physics {

PlaneShape::PlaneShape(glm::vec3 normal, float dist)
    : normal(normal),
      dist(dist)
{
}

PlaneShape::~PlaneShape() {
}

glm::vec3 PlaneShape::getNormal() {
    return normal;
}

float PlaneShape::getDistance() {
    return dist;
}

void PlaneShape::checkCollision(CollisionShape *other, Body *b1, Body *b2,
    std::vector<Contact> & contacts)
{
    Contact contact;

    if (SphereShape *other_sphere = dynamic_cast<SphereShape *>(other)) {
        glm::vec3 p2 = b2->getPosition();
        float dist2 = glm::dot(p2, normal) - dist;

        if (dist2 < other_sphere->getRadius()) {
            contact.b1 = b2;
            contact.b2 = b1;
            contact.normal = normal;
            contact.depth = other_sphere->getRadius() - dist2;
            //contact.position = b1->getPosition() + norm * dist; // TODO

            contacts.push_back(contact);
        }
    }
}

}
