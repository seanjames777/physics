#ifndef __GLDEFS_H
#define __GLDEFS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef __APPLE__
#include <GL/gl.h>
#endif

#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef DEBUG
#define CHECK_GL_ERROR() {\
    GLenum error = glGetError();\
    if (error != GL_NO_ERROR) {\
        printf("GL Error 0x%04x at %s:%d\n", error, __FILE__, __LINE__);\
        exit(-1);\
    }\
}
#else
#define CHECK_GL_ERROR()
#endif

#ifdef util_EXPORTS
#define UTIL_EXPORT __declspec(dllexport)
#else
#define UTIL_EXPORT __declspec(dllimport)
#endif

#pragma warning(disable:4251) // TODO

#endif