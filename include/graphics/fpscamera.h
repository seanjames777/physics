/**
 * @file fpscamera.h
 *
 * @brief First person shooter style yaw/pitch camera
 *
 * @author Sean James
 */

#ifndef __FPSCAMERA_H
#define __FPSCAMERA_H

#include <glm/glm.hpp>
#include <graphics/camera.h>

namespace Graphics {

/**
 * @brief First person shooter style yaw/pitch camera
 */
class FPSCamera : public Camera {
private:

    float yaw;
    float pitch;

    /**
     * @brief Set target
     */
    void setTarget(glm::vec3 target);

    /**
     * @brief Update target when yaw/pitch/position change
     */
    void update();

public:

    /**
     * @brief Constructor
     *
     * @param[in] position Position
     * @param[in] yaw      Rotation about Y axis
     * @param[in] pitch    Rotation about X axis
     */
    FPSCamera(glm::vec3 position, float yaw, float pitch);

    /**
     * @brief Destructor
     */
    ~FPSCamera();

    /**
     * @brief Set position
     */
    void setPosition(glm::vec3 position);

    /**
     * @brief Get rotation about Y axis
     */
    float getYaw();

    /**
     * @brief Get rotation about X axis
     */
    float getPitch();

    /**
     * @brief Set rotation about Y axis
     */
    void setYaw(float yaw);

    /**
     * @brief Set rotation about X axis
     */
    void setPitch(float pitch);

};

}

#endif
