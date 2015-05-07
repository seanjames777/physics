#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <util/defs.h>
#include <memory>
#include <string>

enum TextureFilterMode {
    TextureFilterModeNearest,
    TextureFilterModeBilinear,
    TextureFilterModeTrilinear,
    TextureFilterModeAnisotropic
};

enum TextureAddressMode {
    TextureAddressModeClamp  = GL_CLAMP_TO_EDGE,
    TextureAddressModeWrap   = GL_REPEAT,
    TextureAddressModeMirror = GL_MIRRORED_REPEAT
};

enum SurfaceFormat {
    SurfaceFormatR8         = GL_R8,
    SurfaceFormatR16        = GL_R16,
    SurfaceFormatRGB8       = GL_RGB8,
    SurfaceFormatRGBA8      = GL_RGBA8,
    SurfaceFormatRGB8_SRGB  = GL_SRGB8,
    SurfaceFormatRGBA8_SRGB = GL_SRGB8_ALPHA8,
    SurfaceFormatR16F       = GL_R16F,
    SurfaceFormatR32F       = GL_R32F,
    SurfaceFormatDepth24    = GL_DEPTH_COMPONENT24
};

class UTIL_EXPORT Surface {
public:

    Surface();

    virtual ~Surface();

    virtual bool isTexture() = 0;

    virtual GLuint getGLObject() = 0;

    virtual SurfaceFormat getFormat() = 0;

};

class UTIL_EXPORT Texture : public Surface {
private:

    GLuint        texture;
    int           width;
    int           height;
    int           miplevels;
    SurfaceFormat format;

public:

    Texture(
        SurfaceFormat      format,
        int                width,
        int                height,
        int                miplevels,
        TextureFilterMode  filterMode,
        float              maxAnisotropy,
        TextureAddressMode addressU,
        TextureAddressMode addressV);

    ~Texture();

    void bind(int idx);

    void unbind(int idx);

    void setData(SurfaceFormat dataFormat, void *data);

    virtual SurfaceFormat getFormat() override;

    static std::shared_ptr<Texture> fromJpeg(
        const std::string & filename,
        int                 miplevels,
        TextureFilterMode   filterMode,
        float               maxAnisotropy,
        TextureAddressMode  addressU,
        TextureAddressMode  addressV);

    virtual bool isTexture() override;

    virtual GLuint getGLObject() override;

};

class UTIL_EXPORT RenderBuffer : public Surface {
private:

    GLuint        renderBuffer;
    SurfaceFormat format;

public:

    RenderBuffer(
        SurfaceFormat format,
        int           width,
        int           height);

    ~RenderBuffer();

    virtual SurfaceFormat getFormat() override;

    virtual bool isTexture() override;

    virtual GLuint getGLObject() override;

    void bind();

    void unbind();

};

#endif
