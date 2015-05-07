#ifndef __CAMERA_H
#define __CAMERA_H

#include <util/defs.h>

class UTIL_EXPORT FPSCamera {
private:

    glm::vec3 position;
    float     yaw;
    float     pitch;
    float     aspectRatio;
    bool      dirty;
    glm::mat4 viewProjection;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 target;

public:

    FPSCamera(glm::vec3 position, float yaw, float pitch);

    ~FPSCamera();

    float getYaw();

    float getPitch();

    void setYaw(float yaw);

    void setPitch(float pitch);

    void setAspectRatio(float aspectRatio);

    void setPosition(glm::vec3 position);

    glm::vec3 getPosition();

    glm::vec3 getRight();

    glm::vec3 getForward();

    glm::mat4 getViewProjection();

    glm::vec3 getTarget();

};

#endif
