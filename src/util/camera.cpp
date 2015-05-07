#include <util/camera.h>

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
    : position(position),
      yaw(yaw),
      pitch(pitch),
      aspectRatio(1.0f),
      dirty(true)
{
    // Generate initial values
    getViewProjection();
}

FPSCamera::~FPSCamera() {
}

float FPSCamera::getYaw() {
    return yaw;
}

float FPSCamera::getPitch() {
    return pitch;
}

void FPSCamera::setYaw(float yaw) {
    this->yaw = yaw;
    dirty = true;
}

void FPSCamera::setPitch(float pitch) {
    this->pitch = pitch;
    dirty = true;
}

void FPSCamera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    dirty = true;
}

void FPSCamera::setPosition(glm::vec3 position) {
    this->position = position;
    dirty = true;
}

glm::vec3 FPSCamera::getForward() {
    return forward;
}

glm::vec3 FPSCamera::getRight() {
    return right;
}

glm::vec3 FPSCamera::getTarget() {
    return target;
}

glm::vec3 FPSCamera::getPosition() {
    return position;
}

glm::mat4 FPSCamera::getViewProjection() {
    if (!dirty)
        return viewProjection;

    glm::mat4 rotation = glm::yawPitchRoll(yaw, pitch, 0.0f);
    forward = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
    target = position + forward;

    right = glm::cross(forward, glm::vec3(0, 1, 0));
    glm::vec3 up = glm::cross(right, forward);

    glm::mat4 view = glm::lookAt(position, target, up);

    glm::mat4 projection = glm::perspective((float)M_PI / 4.0f, aspectRatio,
        1.0f, 1000.0f);

    viewProjection = projection * view;
    dirty = false;

    return viewProjection;
}
