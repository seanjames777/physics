/**
 * @file textureutil.h
 *
 * @brief Utilities for loading textures
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

#ifndef __TEXTUREUTIL_H
#define __TEXTUREUTIL_H

#include <GL/glew.h>
#include <memory>
#include <string>

namespace Graphics {

class Texture;

/**
 * @brief CPU memory representation of a texture, which can be loaded into an
 * OpenGL texture or used directly for, e.g. heightmaps.
 */
struct TextureData {
    int            width;    //!< Image width
    int            height;   //!< Image height
    unsigned char *data;     //!< Image data
    GLenum         format;   //!< Image format
    GLenum         dataType; //!< Image data type

    /**
     * @brief Constructor
     *
     * @param[in] width    Image width
     * @param[in] height   Image height
     * @param[in] data     Image data, which will be deleted by the TextureData object
     * @param[in] format   OpenGL image format
     * @param[in] dataType OpenGL image datat type
     */
    TextureData(int width, int height, unsigned char *data, GLenum format,
        GLenum dataType);

    /**
     * @brief Destructor
     */
    ~TextureData();
};

namespace TextureUtil {

/**
 * @brief Load an image from a PNG file. If the image is loaded successfully,
 * the caller is responsible for deleting the texture data.
 *
 * @param[in]  file   PNG filename
 * @param[out] image  Loaded image
 *
 * @return Texture data if the file is loaded successfully, or null otherwise
 */
std::shared_ptr<TextureData> loadPNG(std::string file);

/**
 * @brief Load an image from a JPEG file.
 *
 * @param[in]  file   JPEG filename
 *
 * @return Texture data if the file is loaded successfully, or null otherwise
 */
std::shared_ptr<TextureData> loadJPEG(std::string file);

/**
 * @brief Create a texture from loaded image data
 *
 * @param[in]  data    Texture data
 * @param[in]  format  Format of texture. Must be a legal destination for the
 *                     source image format.
 * @param[in]  levels  Number of mipmap levels to create
 */
std::shared_ptr<Texture> createTextureFromTextureData(
    std::shared_ptr<TextureData> data, GLenum format, int levels);

}}

#endif
