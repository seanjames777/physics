/**
 * @file camera.h
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/graphics/camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Physics { namespace Util {

void Camera::updateView() {
    view = glm::lookAt(position, target, up);
    viewProjection = projection * view;
}

void Camera::updateProjection() {
    projection = glm::perspective(fov, aspect, near, far);
    viewProjection = projection * view;
}

Camera::Camera(glm::vec3 position, glm::vec3 target)
    : position(position),
      target(target),
      up(glm::vec3(0, 1, 0)),
      fov(45.0f),
      aspect(1.0f),
      near(1.0f),
      far(400.0f),
      view(),
      projection(),
      viewProjection()
{
    updateView();
    updateProjection();
}

Camera::~Camera() {
}

glm::mat4 Camera::getView() {
    return view;
}

glm::mat4 Camera::getProjection() {
    return projection;
}

glm::mat4 Camera::getViewProjection() {
    return viewProjection;
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getTarget() {
    return target;
}

void Camera::setPosition(glm::vec3 position) {
    this->position = position;
    updateView();
}

void Camera::setTarget(glm::vec3 target) {
    this->target = target;
    updateView();
}

void Camera::setAspectRatio(float aspect) {
    this->aspect = aspect;
    updateProjection();
}

}}
