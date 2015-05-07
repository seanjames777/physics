/**
 * @file collisionshape.h
 *
 * @brief Shape which can be checked for collisions
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SHAPE_H
#define __SHAPE_H

#include <physics/defs.h>

namespace Physics {

/**
 * @brief Shape which can be checked for collision against other shapes. Shapes are
 * attached to Bodies to provide collision geometry. This is an abstract base class
 * for other Shapes.
 *
 * ShapeType is used to identify the types of shapes to determine which collision
 * functions need to be called. Using an enumeration for this allows us to avoid
 * requiring RTTI. Care has been taken to avoid virtual functions for performance
 * reasons.
 */
class PHYSICS_EXPORT Shape {
public:

    /**
     * @brief Shape types, used to avoid requiring RTTI
     */
    enum ShapeType {
        // Note: When adding to this table, update dispatchTable in collision.cpp.
        Sphere,  //!< Sphere shape type
        Plane,   //!< Plane shape type
        Cube,    //!< Cube shape type
        Count    //!< Number of shapes
    };

private:

    enum ShapeType shapeType; //!< Shape type

public:

    /**
     * @brief Constructor
     */
    Shape(enum ShapeType type);

    /**
     * @brief Destructor
     */
    virtual ~Shape() = 0;

    /**
     * @brief Get this shape's type
     */
    enum ShapeType getShapeType() const;

    /**
     * @brief Get a bounding box for this shape. This function calls the
     * central bounding box computation functions as a convenience. TODO inline
     * this.
     *
     * @param[in]  t    Transform of the body corresponding to this shape
     * @param[out] bbox Bounding box
     */
    // void getBoundingBox(const Transform & t, AABB & bbox);

};

}

#endif
