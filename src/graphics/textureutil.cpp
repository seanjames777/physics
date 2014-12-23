/**
 * @file textureutil.cpp
 *
 * @author Sean James
 *
 * Copyright (c) 2014 Sean James <seanjames777@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software
 *      in a product, an acknowledgment in the product documentation would
 *      be appreciated but is not required.
 *   2. Altered source versions must be plainly marked as such, and must not
 *      be misrepresented as being the original software.
 *   3. This notice may not be removed or altered from any source
 *      distribution.
 */

#include <graphics/textureutil.h>
#include <graphics/texture.h>
#include <png.h>
#include <jpeglib.h>
#include <stdio.h>
#include <graphics/path.h>

namespace Graphics {

TextureData::TextureData(int width, int height, unsigned char *data, GLenum format,
    GLenum dataType)
    : width(width),
      height(height),
      data(data),
      format(format),
      dataType(dataType)
{
}

TextureData::~TextureData() {
    delete [] data;
}

namespace TextureUtil {

std::shared_ptr<TextureData> loadPNG(std::string file) {
    #define HEADERBYTES 8
    png_byte *rows = NULL, color_type;
    png_bytep *row_ptrs = NULL;
    int rowbytes;
    png_infop info_ptr;
    png_structp png_ptr = NULL;
    FILE *fp = NULL;
    int width = 0;
    int height = 0;
    std::shared_ptr<TextureData> texData = nullptr;

#if defined(WIN32) && (!defined(_MT) || !defined(_DLL))
#error "Must use multithreaded DLL"
#endif

    std::string filename = PathUtil::prependExecutableDirectory(file);

    fp = fopen(filename.c_str(), "rb");

    if (fp == NULL) {
        printf("Error opening '%s'\n", filename.c_str());
        goto fail;
    }

    png_byte header[HEADERBYTES];
    fread(header, 1, HEADERBYTES, fp);

    if (png_sig_cmp(header, 0, HEADERBYTES)) {
        printf("%s is not a PNG file\n", filename.c_str());
        goto fail;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL) {
        printf("png_create_read_struct failed\n");
        goto fail;
    }

    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr == NULL) {
        printf("png_create_info_struct failed\n");
        goto fail;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("setjmp returned 0\n");
        goto fail;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, HEADERBYTES);
    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);

    // TODO don't need to add alpha if it's just filling up memory
    if (color_type == PNG_COLOR_TYPE_RGB)
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(png_ptr, info_ptr);

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    row_ptrs = (png_bytep *)malloc(sizeof(png_bytep) * height);
    rows = (png_byte *)malloc(sizeof(png_byte) * height * rowbytes);

    for (int y = 0; y < height; y++)
        row_ptrs[y] = rows + y * rowbytes;

    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("setjmp 2 returned 0\n");
        goto fail;
    }

    png_read_image(png_ptr, row_ptrs);

    texData = std::make_shared<TextureData>(
        width, height, rows, GL_RGBA, GL_UNSIGNED_BYTE);

    // TODO copy to get rid of alignment

    free(row_ptrs);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return texData;

fail:

    if (rows != NULL)
        free(rows);

    if (row_ptrs != NULL)
        free(row_ptrs);

    if (png_ptr != NULL && info_ptr != NULL)
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    else if (png_ptr != NULL)
        png_destroy_read_struct(&png_ptr, NULL, NULL);

    if (fp != NULL)
        fclose(fp);

    return nullptr;
}

std::shared_ptr<TextureData> loadJPEG(std::string filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_ptr;

    filename = PathUtil::prependExecutableDirectory(filename);

    FILE *file = fopen(filename.c_str(), "rb");

    unsigned long location = 0;
    int i = 0;

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

    std::shared_ptr<TextureData> texData = std::make_shared<TextureData>(
        cinfo.output_width, cinfo.output_height, img, GL_RGB, GL_UNSIGNED_BYTE);

    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    free(row_ptr);

    fclose(file);

    return texData;
}

std::shared_ptr<Texture> createTextureFromTextureData(
    std::shared_ptr<TextureData> data, GLenum format, int levels)
{
    return std::make_shared<Texture>(data->width, data->height,
        format, data->dataType, levels, data->data, data->format);
}

}}
