#ifndef __RENDERTARGET_H
#define __RENDERTARGET_H

#include <util/texture.h>
#include <memory>

#define MAX_COLOR_TARGETS 4

class UTIL_EXPORT RenderTarget {
private:

    GLuint                   framebuffer;
    std::shared_ptr<Surface> colorTargets[MAX_COLOR_TARGETS];
    int                      numColorTargets;
    std::shared_ptr<Surface> depthStencilTarget;

public:

    RenderTarget();

    ~RenderTarget();

    void addColorTarget(std::shared_ptr<Surface> texture);

    void addDepthStencilTarget(std::shared_ptr<Surface> texture);

    void finish();

    void bind();

    void unbind();

};

#endif
