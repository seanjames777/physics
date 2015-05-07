#include <util/texture.h>
#include <jpeglib.h>

Surface::Surface() {
}

Surface::~Surface() {
}

Texture::Texture(
    SurfaceFormat      format,
    int                width,
    int                height,
    int                miplevels,
    TextureFilterMode  filterMode,
    float              maxAnisotropy,
    TextureAddressMode addressU,
    TextureAddressMode addressV)
    : width(width),
      height(height),
      miplevels(miplevels),
      format(format)
{
    assert(miplevels >= 1);

    glGenTextures(1, &texture);

    bind(0);

    glTexStorage2D(GL_TEXTURE_2D, miplevels, (GLenum)format, width, height);

    switch(filterMode) {
    case TextureFilterModeNearest:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case TextureFilterModeBilinear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			miplevels > 1 ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case TextureFilterModeTrilinear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
        break;
    case TextureFilterModeAnisotropic:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
        break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)addressU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)addressV);

    unbind(0);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

SurfaceFormat Texture::getFormat() {
    return format;
}

void Texture::bind(int idx) {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind(int idx) {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setData(SurfaceFormat dataFormat, void *data) {
    bind(0);

    GLenum format = 0;
    GLenum type = 0;

    switch(dataFormat) {
    case SurfaceFormatRGB8_SRGB:
    case SurfaceFormatRGB8:
        format = GL_RGB;
        break;
    case SurfaceFormatRGBA8_SRGB:
    case SurfaceFormatRGBA8:
        format = GL_RGBA;
        break;
    case SurfaceFormatR8:
    case SurfaceFormatR16:
    case SurfaceFormatR16F:
    case SurfaceFormatR32F:
        format = GL_R;
        break;
    case SurfaceFormatDepth24:
        break;
    }

    switch(dataFormat) {
    case SurfaceFormatRGB8_SRGB:
    case SurfaceFormatRGB8:
    case SurfaceFormatRGBA8_SRGB:
    case SurfaceFormatRGBA8:
    case SurfaceFormatR8:
        type = GL_UNSIGNED_BYTE;
        break;
    case SurfaceFormatR16:
        type = GL_UNSIGNED_SHORT;
        break;
    case SurfaceFormatR16F:
    case SurfaceFormatR32F:
        type = GL_FLOAT;
        break;
    case SurfaceFormatDepth24:
        break;
    }

    assert(format != 0 && type != 0);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
        format, type, data);

    CHECK_GL_ERROR();

    if (miplevels > 1)
        glGenerateMipmap(GL_TEXTURE_2D);

    unbind(0);
}

std::shared_ptr<Texture> Texture::fromJpeg(
    const std::string & filename,
    int                 miplevels,
    TextureFilterMode   filterMode,
    float               maxAnisotropy,
    TextureAddressMode  addressU,
    TextureAddressMode  addressV)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_ptr;

    FILE *file = fopen(filename.c_str(), "rb");

    if (!file) {
        printf("Error opening '%s'\n", filename.c_str());
        return nullptr;
    }

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, file);

    jpeg_read_header(&cinfo, true);

    jpeg_start_decompress(&cinfo);

    unsigned char *img = (unsigned char *)malloc(cinfo.output_width *
        cinfo.output_height * cinfo.num_components);

    row_ptr = (unsigned char *)malloc(cinfo.output_width *
        cinfo.num_components);

    while (cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, &row_ptr, 1);

        memcpy(img + (cinfo.output_scanline - 1) * cinfo.output_width *
            cinfo.output_components, row_ptr,
            cinfo.output_width * cinfo.output_components);
    }

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(
        SurfaceFormatRGB8_SRGB,
        cinfo.output_width,
        cinfo.output_height,
        miplevels,
        filterMode,
        maxAnisotropy,
        addressU,
        addressV);

    texture->setData(SurfaceFormatRGB8_SRGB, img);

    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    free(row_ptr);
    free(img);

    fclose(file);

    return texture;
}

bool Texture::isTexture() {
    return true;
}

GLuint Texture::getGLObject() {
    return texture;
}

RenderBuffer::RenderBuffer(
    SurfaceFormat format,
    int           width,
    int           height)
    : format(format)
{
    glGenRenderbuffers(1, &renderBuffer);

    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)format, width, height);
    unbind();
}

RenderBuffer::~RenderBuffer() {
    glDeleteRenderbuffers(1, &renderBuffer);
}

bool RenderBuffer::isTexture() {
    return false;
}

SurfaceFormat RenderBuffer::getFormat() {
    return format;
}

GLuint RenderBuffer::getGLObject() {
    return renderBuffer;
}

void RenderBuffer::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
}

void RenderBuffer::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
