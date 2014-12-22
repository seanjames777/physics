/**
 * @file texture.h
 *
 * @brief Texture wrapper class
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

#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <GL/glew.h>

// TODO:
//     - When creating the texture, we bind and unbind four times
//     - Make render targets use this class or something

namespace Graphics {

/**
 * @brief Texture class. Primarily a wrapper around OpenGL 2D textures.
 */
class Texture {
private:

    GLuint texture;  //!< OpenGL texture
    int    width;    //!< Width
    int    height;   //!< Height
    GLenum format;   //!< Texture format
    GLenum dataType; //!< Texture data type
    int    levels;   //!< Number of mipmap levels

public:

    /*
     * @brief Constructor. Note that this constructor will unbind the texture
     * currently bound to texture slot 0, if there is one.
     *
     * @param[in] width       Width
     * @param[in] height      Height
     * @param[in] format      Texture format, i.e. GL_RGBA
     * @param[in] type        Texture data type, i.e. GL_UNSIGNED_BYTE
     * @param[in] levels      Number of mipmap levels
     * @param[in] data        Pixel data, possibly NULL
     * @param[in] inputFormat The format "data" is in
     */
    Texture(int width, int height, GLenum format, GLenum dataType, int levels,
        void *data = NULL, GLenum inputFormat = GL_RGBA8);

    /**
     * @brief Destructor
     */
    ~Texture();

    /**
     * @brief Bind the texture for reading
     *
     * @param[in] idx Texture index to bind to
     */
    void bind(int idx);

    /**
     * @brief Unbind the texture
     */
    void unbind(int idx);

    /**
     * @brief Set the filter mode to use when reading from the texture in a
     * shader. Note that this function will unbind the texture currently bound
     * to texture slot 0, if there is one.
     *
     * @param[in] minFilter  Minification filter mode
     * @param[in] magFilter  Magnification filter mode
     * @param[in] anisotropy Maximum anisotropy
     */
    void setFilterMode(GLenum minFilter, GLenum magFilter, float anisotropy = 0.0f);

    /**
     * @brief Set the address mode to use when reading from the texture in a
     * shader. This determines how UV coordinates that fall outside of the 0
     * to 1 range will be treated. Note that this function will unbind the
     * texture currently bound to texture slot 0, if there is one.
     *
     * @param[in] addrU Address mode for S axis
     * @param[in] addrV Address mode for T axis
     */
    void setAddressMode(GLenum addrS, GLenum addrT);

    /**
     * @brief Set the texture data
     *
     * @param[in] data        New texture data
     * @oaram[in] inputFormat The format "data" is in
     */
    void setData(void *data, GLenum inputFormat);

};

}

#endif
