/**
 * @file planeshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/collision/planeshape.h>
#include <physics/collision/sphereshape.h>
#include <iostream> // TODO
#include <physics/dynamics/body.h>

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
            contact.position = b2->getPosition() - normal * dist2;

            contacts.push_back(contact);
        }
    }
}

void PlaneShape::getBoundingBox(Body *body, glm::vec3 & min, glm::vec3 & max) {
    min = glm::vec3(-1.0 / 0.0, 0.0f, -1.0 / 0.0); // TODO
    min = glm::vec3( 1.0 / 0.0, 0.0f,  1.0 / 0.0);
}

}
