/**
 * @file texture.cpp
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

#include <graphics/texture.h>
#include <cassert>

namespace Graphics {

Texture::Texture(int width, int height, GLenum format, GLenum dataType, int levels,
    void *data, GLenum inputFormat)
    : width(width),
      height(height),
      format(format),
      dataType(dataType),
      levels(levels)
{
    glGenTextures(1, &texture);

    setFilterMode(levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR, GL_LINEAR, 0.0f);
    setAddressMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    if (data)
        setData(data, inputFormat);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

void Texture::bind(int idx) {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind(int idx) {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setFilterMode(GLenum minFilter, GLenum magFilter, float anisotropy) {
    bind(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    // TODO
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

    unbind(0);
}

void Texture::setAddressMode(GLenum addrS, GLenum addrT) {
    bind(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, addrS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, addrT);

    unbind(0);
}

void Texture::setData(void *data, GLenum inputFormat) {
    bind(0);

    int currWidth = width;
    int currHeight = height;

    for (int i = 0; i < levels; i++) {
        glTexImage2D(GL_TEXTURE_2D, i, inputFormat, currWidth, currHeight, 0,
            format, dataType, i == 0 ? data : NULL);

        currWidth /= 2;
        currHeight /= 2;
    }

    if (levels > 1)
        glGenerateMipmap(GL_TEXTURE_2D);

    unbind(0);
}

}
