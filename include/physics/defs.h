#ifndef __PHYSICS_DEFS_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef physics_EXPORTS
#define PHYSICS_EXPORT __declspec(dllexport)
#else
#define PHYSICS_EXPORT __declspec(dllimport)
#endif

#pragma warning(disable:4251) // TODO

#endif
