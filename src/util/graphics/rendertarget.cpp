/**
 * @file rendertarget.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <util/graphics/rendertarget.h>
#include <cassert>
#include <iostream> // TODO

namespace Physics { namespace Util {

RenderTarget::RenderTarget(int width, int height, int samples)
    : width(width),
      height(height),
      samples(samples),
      depthStencilBuffer(0),
      frameBuffer(0),
      numColorBuffers(0),
      depthBufferEnabled(false),
      stencilBufferEnabled(false)
{
    glGenFramebuffers(1, &frameBuffer);
}

RenderTarget::~RenderTarget() {
    if (numColorBuffers)
        glDeleteTextures(numColorBuffers, colorBuffers);

    if (depthBufferEnabled || stencilBufferEnabled)
        glDeleteTextures(1, &depthStencilBuffer);

    glDeleteBuffers(1, &frameBuffer);
}

void RenderTarget::addColorTarget(GLenum format, GLenum filter) {
    assert(numColorBuffers < MAX_COLOR_BUFF);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);

    if (samples > 0) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format,
            width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    colorBuffers[numColorBuffers++] = texture;
}

bool RenderTarget::formatHasDepthComponent(GLenum format) {
    switch(format) {
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32F:
    case GL_DEPTH24_STENCIL8:
    case GL_DEPTH32F_STENCIL8:
    // TODO ...
        return true;
    default:
        return false;
    }
}

bool RenderTarget::formatHasStencilComponent(GLenum format) {
    switch(format) {
    case GL_DEPTH24_STENCIL8:
    case GL_DEPTH32F_STENCIL8:
    // TODO ...
        return true;
    default:
        return false;
    }
}

void RenderTarget::addDepthStencilTarget(GLenum format) {
    glGenTextures(1, &depthStencilBuffer);
    glActiveTexture(GL_TEXTURE0);

    if (samples > 0) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthStencilBuffer);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format,
            width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, depthStencilBuffer);
        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    depthBufferEnabled = formatHasDepthComponent(format);
    stencilBufferEnabled = formatHasStencilComponent(format);
}

void RenderTarget::finish() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    for (int i = 0; i < numColorBuffers; i++)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
            colorBuffers[i], 0);

    if (depthBufferEnabled)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            depthStencilBuffer, 0);

    if (stencilBufferEnabled)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
            depthStencilBuffer, 0);

    assert(glGetError() == GL_NO_ERROR);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(status == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    GLenum drawBuffers[MAX_COLOR_BUFF];

    for (int i = 0; i < numColorBuffers; i++)
        drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

    glDrawBuffers(numColorBuffers, drawBuffers);
}

void RenderTarget::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::bindColorTexture(int targetIdx, int sourceIdx) {
    glActiveTexture(GL_TEXTURE0 + targetIdx);

    if (samples == 0)
        glBindTexture(GL_TEXTURE_2D, colorBuffers[sourceIdx]);
    else
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBuffers[sourceIdx]);
}

void RenderTarget::bindDepthStencilTexture(int targetIdx) {
    glActiveTexture(GL_TEXTURE0 + targetIdx);

    if (samples == 0)
        glBindTexture(GL_TEXTURE_2D, depthStencilBuffer);
    else
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthStencilBuffer);
}

void RenderTarget::unbindColorTexture(int targetIdx) {
    glActiveTexture(GL_TEXTURE0 + targetIdx);

    if (samples == 0)
        glBindTexture(GL_TEXTURE_2D, 0);
    else
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void RenderTarget::unbindDepthStencilTexture(int targetIdx) {
    glActiveTexture(GL_TEXTURE0 + targetIdx);

    if (samples == 0)
        glBindTexture(GL_TEXTURE_2D, 0);
    else
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

int RenderTarget::getWidth() {
    return width;
}

int RenderTarget::getHeight() {
    return height;
}

int RenderTarget::getSamples() {
    return samples;
}

void RenderTarget::blit(int x0, int y0, int w, int h, int colorIdx) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + colorIdx);

    glBlitFramebuffer(0, 0, width, height, x0, y0, w, h, GL_COLOR_BUFFER_BIT,
        GL_NEAREST); // TODO filter

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

}}
