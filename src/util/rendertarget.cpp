#include <util/rendertarget.h>

RenderTarget::RenderTarget()
    : numColorTargets(0),
      depthStencilTarget(nullptr)
{
    glGenFramebuffers(1, &framebuffer);
}

RenderTarget::~RenderTarget() {
    glDeleteFramebuffers(1, &framebuffer);
}


void RenderTarget::addColorTarget(std::shared_ptr<Surface> texture) {
    colorTargets[numColorTargets++] = texture;
}

void RenderTarget::addDepthStencilTarget(std::shared_ptr<Surface> texture) {
    depthStencilTarget = texture;
}

void RenderTarget::finish() {
    bind();

    for (int i = 0; i < numColorTargets; i++) {
        std::shared_ptr<Surface> target = colorTargets[i];

        if (target->isTexture())
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                target->getGLObject(), 0);
        else
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                GL_RENDERBUFFER, target->getGLObject());
    }

    bool depth = false;
    bool stencil = false;

    if (depthStencilTarget) {
        switch (depthStencilTarget->getFormat()) {
        case SurfaceFormatR8:
        case SurfaceFormatR16:
        case SurfaceFormatRGB8:
        case SurfaceFormatRGBA8:
        case SurfaceFormatRGB8_SRGB:
        case SurfaceFormatRGBA8_SRGB:
        case SurfaceFormatR16F:
        case SurfaceFormatR32F:
            break;
        case SurfaceFormatDepth24:
            depth = true;
            break;
        }

        assert(depth || stencil);
    }

    if (depth) {
        if (depthStencilTarget->isTexture())
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                depthStencilTarget->getGLObject(), 0);
        else
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER, depthStencilTarget->getGLObject());
    }

    if (stencil) {
        if (depthStencilTarget->isTexture())
            glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                depthStencilTarget->getGLObject(), 0);
        else
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER, depthStencilTarget->getGLObject());
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);

    unbind();
}

void RenderTarget::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void RenderTarget::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
