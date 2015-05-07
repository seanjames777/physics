/**
 * @file collision.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <physics/collision/collision.h>
#include <physics/collision/sphereshape.h>
#include <physics/collision/planeshape.h>
#include <physics/collision/cubeshape.h>
#include <iostream>

namespace Physics {
namespace Collision {

// TODO collision function
typedef bool (*collisionFunc)(const Shape &, const Shape &, const Transform &,
    const Transform &, Contact &);

// TODO
static collisionFunc dispatchTable[Shape::Count][Shape::Count];
static bool initialized = false;

// TODO header
bool checkCollisionSphereSphere(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    const SphereShape & sphere1 = static_cast<const SphereShape &>(s1);
    const SphereShape & sphere2 = static_cast<const SphereShape &>(s2);

    glm::vec3 diff = t2.position - t1.position;
    float r1 = sphere1.getRadius();
    float r2 = sphere2.getRadius();

    float dist = glm::length(diff); // TODO could use dist squared maybe

    if (dist < r1 + r2) {
        glm::vec3 norm = diff / dist;

        contact.normal = norm;
        contact.depth = r1 + r2 - dist;
        contact.position = t1.position + norm * r1;

        return true;
    }

    return false;
}

bool checkCollisionSpherePlane(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    const SphereShape & sphere = static_cast<const SphereShape &>(s1);
    const PlaneShape & plane = static_cast<const PlaneShape &>(s2);

    glm::vec3 p1 = t1.position;
    glm::vec3 norm = plane.getNormal();
    float planeDist = plane.getDistance();
    float sphereR = sphere.getRadius();

    float dist = glm::dot(p1, norm) - planeDist;

    if (dist < sphereR) {
        contact.normal = -norm;
        contact.depth = sphereR - dist;
        contact.position = p1 + contact.normal * (sphereR - contact.depth);

        return true;
    }

    return false;
}

bool checkCollisionPlaneSphere(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    if (checkCollisionSpherePlane(s2, s1, t2, t1, contact)) {
        contact.normal = -contact.normal; // TODO
        return true;
    }

    return false;
}

bool checkCollisionSphereCube(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    return false;
}

bool checkCollisionPlanePlane(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    return false;
}

bool checkCollisionCubeSphere(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    return false;
}

bool checkCollisionCubePlane(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    const CubeShape & cube = static_cast<const CubeShape &>(s1);
    const PlaneShape & plane = static_cast<const PlaneShape &>(s2);

    float planeDist = plane.getDistance();
    glm::vec3 norm = plane.getNormal();

    glm::vec3 delta = glm::vec3(cube.getWidth(), cube.getHeight(), cube.getDepth()) / 2.0f;

    // TODO make this better
    glm::vec3 points[8];
    points[0] = glm::vec3(-delta.x, -delta.y, -delta.z);
    points[1] = glm::vec3( delta.x, -delta.y, -delta.z);
    points[2] = glm::vec3(-delta.x,  delta.y, -delta.z);
    points[3] = glm::vec3( delta.x,  delta.y, -delta.z);
    points[4] = glm::vec3(-delta.x, -delta.y,  delta.z);
    points[5] = glm::vec3( delta.x, -delta.y,  delta.z);
    points[6] = glm::vec3(-delta.x,  delta.y,  delta.z);
    points[7] = glm::vec3( delta.x,  delta.y,  delta.z);

    // Transform points to world space
    for (int i = 0; i < 8; i++)
        t1.transform(points[i]);

    float maxDepth = 0.0f;
    glm::vec3 maxPoint;
    bool found = false;

    // Check each point. TODO: check if the max depth is the best. Might be able
    // to merge with above loop to avoid some math.
    for (int i = 0; i < 8; i++) {
        float depth = -(glm::dot(points[i], norm) - planeDist); // TODO could move negative

        if (depth > 0.0f && depth > maxDepth) {
            maxDepth = depth;
            maxPoint = points[i];
            found = true;
        }
    }

    if (found) {
        contact.position = maxPoint;
        contact.depth = maxDepth;
        contact.normal = -norm;

        return true;
    }

    return false;
}

bool checkCollisionPlaneCube(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    if (checkCollisionCubePlane(s2, s1, t2, t1, contact)) {
        contact.normal = -contact.normal;
        return true;
    }

    return false;
}

bool checkCollisionCubeCube(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    return false;
}

bool checkCollisionUndefined(const Shape & s1, const Shape & s2,
    const Transform & t1, const Transform & t2, Contact & contact)
{
    assert(false && "Dispatch table missing an entry");
	return false;
}

void initialize() {
    if (initialized)
        return;

    initialized = true;

    // Make sure any missing functions will cause an error
    for (int i = 0; i < Shape::Count; i++)
        for (int j = 0; j < Shape::Count; j++)
            dispatchTable[i][j] = checkCollisionUndefined;

    // Set up dispatch table
    dispatchTable[Shape::Sphere][Shape::Sphere] = checkCollisionSphereSphere;
    dispatchTable[Shape::Sphere][Shape::Plane]  = checkCollisionSpherePlane;
    dispatchTable[Shape::Sphere][Shape::Cube]   = checkCollisionSphereCube;
    dispatchTable[Shape::Plane][Shape::Sphere]  = checkCollisionPlaneSphere;
    dispatchTable[Shape::Plane][Shape::Plane]   = checkCollisionPlanePlane;
    dispatchTable[Shape::Plane][Shape::Cube]    = checkCollisionPlaneCube;
    dispatchTable[Shape::Cube][Shape::Sphere]   = checkCollisionCubeSphere;
    dispatchTable[Shape::Cube][Shape::Plane]    = checkCollisionCubePlane;
    dispatchTable[Shape::Cube][Shape::Cube]     = checkCollisionCubeCube;
}

bool checkCollision(const Shape & s1, const Shape & s2, const Transform & t1,
    const Transform & t2, Contact & contact)
{
    Shape::ShapeType s1_type = s1.getShapeType();
    Shape::ShapeType s2_type = s2.getShapeType();

    return dispatchTable[s1_type][s2_type](s1, s2, t1, t2, contact);
}

}}
