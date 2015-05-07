/**
 * @file collision.h
 *
 * @brief Central fine-grained collision detection dispatcher, used to avoid virtual function
 * calls and RTTI, and to organize collision detection into one place.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __COLLISION_H
#define __COLLISION_H

#include <glm/glm.hpp>
#include <physics/transform.h>
#include <physics/collision/shape.h>

namespace Physics {
namespace Collision {

/**
 * @brief Represents a collision/contact point between two bodies, and stores
 * information needed to generate an impulse to separate them.
 */
struct Contact {
    glm::vec3  position; //!< Contact location in world space
    glm::vec3  normal;   //!< Contact normal in world space, pointing from first shape to second
    float      depth;    //!< Contact depth, where positive depth indicated penetration
};

/**
 * @brief Contact manifold, which collects a set of contact points
 */
/*struct Manifold {
    Body                 *b1;       //!< First body
    Body                 *b2;       //!< Second body
    std::vector<Contact>  contacts; //!< Set of contacts
};*/

/**
 * @brief Initialize collision system. This can safely be called more than once.
 */
void PHYSICS_EXPORT initialize(); // TODO static?

/**
 * @brief Check for collision with another shape. This function calls the
 * central fine-collision function as a convenience. TODO inline this.
 *
 * @param[in]  s1      First shape
 * @param[in]  s2      Second shape
 * @param[in]  t1      Transform of first body, corresponding to this shape
 * @param[in]  t2      Transform of second body, corresponding to other shape
 * @param[out] contact Contact, which should be filled out if there was a collision
 *
 * @return True if there was a collision, or false otherwise
 */
bool PHYSICS_EXPORT checkCollision(const Shape & s1, const Shape & s2, const Transform & t1,
    const Transform & t2, Contact & contact);

}}

#endif
