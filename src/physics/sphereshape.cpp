/**
 * @file sphereshape.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/sphereshape.h>
#include <iostream> // TODO
#include <physics/body.h>
#include <physics/planeshape.h>

namespace Physics {

SphereShape::SphereShape(float radius)
    : r(radius)
{
}

SphereShape::~SphereShape() {
}

float SphereShape::getRadius() {
    return r;
}

void SphereShape::checkCollision(CollisionShape *other, Body *b1, Body *b2,
    std::vector<Contact> & contacts)
{
    Contact contact;

    // TODO: dynamic_cast forces rtti
    if (SphereShape *other_sphere = dynamic_cast<SphereShape *>(other)) {
        glm::vec3 diff = b1->getPosition() - b2->getPosition();
        glm::vec3 norm = glm::normalize(diff);
        // TODO: dot might be faster. also elsewhere. squared dist?
        float dist = glm::length(diff);

        if (dist < r + other_sphere->r) {
            contact.b1 = b1;
            contact.b2 = b2;
            contact.normal = norm;
            contact.depth = r + other_sphere->r - dist;
            //contact.position = b1->getPosition() + norm * dist; // TODO

            contacts.push_back(contact);
        }
    }
    else if (PlaneShape *other_plane = dynamic_cast<PlaneShape *>(other)) {
        glm::vec3 p1 = b1->getPosition();
        float dist = glm::dot(p1, other_plane->getNormal()) - other_plane->getDistance();

        if (dist < r) {
            contact.b1 = b1;
            contact.b2 = b2;
            contact.normal = other_plane->getNormal();
            contact.depth = r - dist;
            //contact.position = b1->getPosition() + norm * dist; // TODO

            contacts.push_back(contact);
        }
    }
}

void SphereShape::getBoundingBox(Body *body, glm::vec3 & min, glm::vec3 & max) {
    glm::vec3 position = body->getPosition();
    glm::vec3 rad = glm::vec3(r, r, r);

    min = position - rad;
    max = position + rad;
}

}
