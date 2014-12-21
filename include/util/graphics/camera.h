/**
 * @file camera.h
 *
 * @brief Camera utility
 *
 * @author Sean James
 */

#ifndef __CAMERA_H
#define __CAMERA_H

#include <glm/glm.hpp>

namespace Physics { namespace Util {

/**
 * @brief Camera base class
 */
class Camera {
private:

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float     fov;
    float     aspect;
    float     near;
    float     far;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 viewProjection;

    /**
     * @brief Update the view matrix after position, target, or up have changed
     */
    void updateView();

    /**
     * @brief Update the projection matrix after fov, aspect, near, or far have
     * changed.
     */
    void updateProjection();

public:

    /**
     * @brief Constructor
     *
     * @param[in] position Position
     * @param[in] target   Target
     */
    Camera(glm::vec3 position, glm::vec3 target);

    /**
     * @brief Destructor
     */
    ~Camera();

    /**
     * @brief Get view matrix
     */
    glm::mat4 getView();

    /**
     * @brief Get projection matrix
     */
    glm::mat4 getProjection();

    /**
     * @brief Get view multiplied by projection
     */
    glm::mat4 getViewProjection();

    /**
     * @brief Get position
     */
    glm::vec3 getPosition();

    /**
     * @brief Get target
     */
    glm::vec3 getTarget();

    /**
     * @brief Set position
     */
    void setPosition(glm::vec3 position);

    /**
     * @brief Set target
     */
    void setTarget(glm::vec3 target);

    /**
     * @brief Set aspect ratio
     */
    void setAspectRatio(float aspect);

};

}}

#endif
