/**
 * @file rendertarget.h
 *
 * @brief Target for offscreen rendering, with one or more color targets and an
 * optional depth/stencil target.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __RENDERTARGET_H
#define __RENDERTARGET_H

#include <GL/glew.h>

#define MAX_COLOR_BUFF 4

namespace Physics { namespace Util {

/**
 * @brief Target for offscreen rendering
 */
class RenderTarget {
private:

    int    width;                         //!< Width
    int    height;                        //!< Height
    int    samples;                       //!< Multisample count
    GLuint colorBuffers[MAX_COLOR_BUFF];  //!< GL color buffers
    GLuint depthStencilBuffer;            //!< GL depth/stencil buffer
    GLuint frameBuffer;                   //!< GL framebuffer
    int    numColorBuffers;               //!< Number of color buffers
    bool   depthBufferEnabled;            //!< Whether there is a depth buffer
    bool   stencilBufferEnabled;          //!< Whether there is a stencil buffer

    /**
     * @brief Check whether a surface format has a depth component
     */
    static bool formatHasDepthComponent(GLenum format);

    /**
     * @brief Check whether a surface format has a stencil component
     */
    static bool formatHasStencilComponent(GLenum format);

public:

    /**
     * @brief Constructor. Call addColorTarget() and/or addDepthStencilTarget()
     * followed by finish() before using render target.
     *
     * @param[in] width   Width
     * @param[in] height  Height
     * @param[in] samples Multisample count
     */
    RenderTarget(int width, int height, int samples);

    /**
     * @brief Destructor
     */
    ~RenderTarget();

    /**
     * @brief Add a color target. Must be called before finish().
     *
     * @param[in] format Surface format
     * @param[in] filter Texture filter when sampling target
     */
    void addColorTarget(GLenum format, GLenum filter);

    /**
     * @brief Add a depth/stencil target. Must be called before finish().
     *
     * @param[in] format Surface format
     */
    void addDepthStencilTarget(GLenum format);

    /**
     * @brief Finish building the render target. Must be called before
     * render target is used.
     */
    void finish();

    /**
     * @brief Bind render target for drawing
     */
    void bind();

    /**
     * @brief Un-bind render target
     */
    void unbind();

    /**
     * @brief Bind color target for sampling
     *
     * @param[in] targetIdx 2D texture index to bind to
     * @param[in] sourceIdx Color channel to bind
     */
    void bindColorTexture(int targetIdx, int sourceIdx);

    /**
     * @brief Bind depth/stencil target for sampling
     *
     * @param[in] targetIdx 2D texture index to bind to
     */
    void bindDepthStencilTexture(int targetIdx);

    /**
     * @brief Unbind color texture
     *
     * @param[in] targetIdx Index to un-bind from
     */
    void unbindColorTexture(int targetIdx);

    /**
     * @brief Unbind depth/stencil texture
     *
     * @param[in] targetIdx Index to un-bind from
     */
    void unbindDepthStencilTexture(int targetIdx);

    /**
     * @brief Get width
     */
    int getWidth();

    /**
     * @brief Get height
     */
    int getHeight();

    /**
     * @brief Get multisample count
     */
    int getSamples();

    /**
     * @brief Blit render target to backbuffer
     */
    void blit(int x0, int y0, int w, int h, int colorIdx);

};

}}

#endif
