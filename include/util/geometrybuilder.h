#ifndef __GEOMETRYBUILDER_H
#define __GEOMETRYBUILDER_H

#include <util/mesh.h>

class UTIL_EXPORT GeometryBuilder {
public:

    static std::shared_ptr<Mesh> createPlane(
        float width,
        float height);

    static std::shared_ptr<Mesh> createCube(
        float width,
        float height,
        float depth);

    static std::shared_ptr<Mesh> createSphere(
        int nu,
        int nv,
        float r);

};

#endif
