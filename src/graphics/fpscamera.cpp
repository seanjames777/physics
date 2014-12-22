/**
 * @file fpscamera.cpp
 *
 * @author Sean James
 */

#include <graphics/fpscamera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Graphics {

void FPSCamera::update() {
    glm::mat4 rotate =
        glm::rotate(
            glm::rotate(
                glm::mat4(), yaw, glm::vec3(0, 1, 0)),
            pitch, glm::vec3(1, 0, 0));

    glm::vec4 diff = rotate * glm::vec4(0, 0, 1, 1);
    glm::vec3 target = getPosition() + glm::vec3(diff.x, diff.y, diff.z);

    Camera::setTarget(target);
}

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
    : Camera(position, glm::vec3()),
      yaw(yaw),
      pitch(pitch)
{
    update();
}

FPSCamera::~FPSCamera() {
}

void FPSCamera::setPosition(glm::vec3 position) {
    Camera::setPosition(position);
    update();
}

float FPSCamera::getYaw() {
    return yaw;
}

float FPSCamera::getPitch() {
    return pitch;
}

void FPSCamera::setYaw(float yaw) {
    this->yaw = yaw;
    update();
}

void FPSCamera::setPitch(float pitch) {
    this->pitch = pitch;
    update();
}

}
