/**
 * @file font.cpp
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

#include <graphics/font.h>
#include <graphics/mesh.h>
#include <graphics/shader.h>
#include <graphics/path.h>
#include <glm/gtc/type_ptr.hpp>

namespace Graphics {

bool Font::ftInitted = false;
FT_Library Font::library;

FontBuffer::~FontBuffer() {
}

UtilFontBuffer::UtilFontBuffer() {
    bool stat = false;

    shader = new Shader();
    stat = shader->setVS("content/shaders/font.vs");
    assert(stat);
    stat = shader->setFS("content/shaders/font.fs");
    assert(stat);
    stat = shader->link();
    assert(stat);

    mesh = new Mesh();
}

UtilFontBuffer::~UtilFontBuffer() {
    delete shader;
    delete mesh;
}

void UtilFontBuffer::setup(int atlasWidth, int atlasHeight, GLuint atlasTexture) {
    this->atlas = atlasTexture;

    shader->bind();
    glUniform1i(glGetUniformLocation(shader->getProgram(), "atlas"), 0);
    glUniform2fv(glGetUniformLocation(shader->getProgram(), "atlasSz"), 1,
        glm::value_ptr(glm::vec2(atlasWidth, atlasHeight)));
    shader->unbind();
}

void UtilFontBuffer::addVertex(glm::vec2 position, glm::vec2 uv, glm::vec4 color) {
    MeshVertex v;

    v.position = glm::vec3(position, 0.0f);
    v.uv = uv;
    v.color = color;

    vertices.push_back(v);
}

void UtilFontBuffer::addIndex(int idx) {
    indices.push_back(idx);
}

void UtilFontBuffer::flush(int screenW, int screenH) {
    if (vertices.size() == 0)
        return;

    mesh->setVertices(&vertices[0], vertices.size());
    mesh->setIndices(&indices[0], indices.size());
    vertices.clear();
    indices.clear();

    shader->bind();

    glUniform2fv(glGetUniformLocation(shader->getProgram(), "screenSz"), 1,
        glm::value_ptr(glm::vec2(screenW, screenH)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mesh->draw();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, 0);

    shader->unbind();
}

int UtilFontBuffer::numVertices() {
    return vertices.size();
}

int UtilFontBuffer::numIndices() {
    return indices.size();
}

Font::Font(std::string filename, int faceIdx, int ptSize, int lineHeight,
    FontBuffer *buff)
    : lineHeight(lineHeight),
      color(glm::vec4(1, 1, 1, 1))
{
    int status;

    if (!ftInitted) {
        status = FT_Init_FreeType(&library);
        assert(!status);
        ftInitted = true;
    }

    std::string path = PathUtil::prependExecutableDirectory(filename);

    FT_Face face;
    status = FT_New_Face(library, path.c_str(), faceIdx, &face);
    assert(!status);

    status = FT_Set_Char_Size(face, ptSize * 64, ptSize * 64, 96, 96);
    assert(!status);

    createCharacterMap(face);
    createCharacterAtlas(face);

    FT_Done_Face(face);

    if (!buff) {
        buff = new UtilFontBuffer();
        ownBuff = true;
    }

    buff->setup(charMap.atlasWidth, charMap.atlasHeight, atlas);

    this->buff = buff;
}

Font::~Font() {
    // TODO delete atlas

    if (ownBuff)
        delete buff;
}

void Font::createCharacterMap(FT_Face face) {
    memset(&charMap, 0, sizeof(CharMap));

    // Get per-character info and compute the maximum width and height needed
    // to store all characters
    for (int i = MIN_CHAR; i <= MAX_CHAR; i++) {
        int char_w, char_h;

        int idx = FT_Get_Char_Index(face, (unsigned long)i);

        if (!idx)
            continue;

        int status = FT_Load_Glyph(face, idx, FT_LOAD_RENDER);
        assert(!status);

        char_w = face->glyph->bitmap.width;
        char_h = face->glyph->bitmap.rows;

        CharInfo *chr = &charMap.chars[i - MIN_CHAR];

        chr->ftIdx = idx;
        chr->width = char_w;
        chr->height = char_h;
        chr->left = face->glyph->bitmap_left;
        chr->top = face->glyph->bitmap_top;
        chr->advance = face->glyph->advance.x / 64;

        if (char_w > charMap.maxWidth)
            charMap.maxWidth = char_w;

        if (char_h > charMap.maxHeight)
            charMap.maxHeight = char_h;
    }

    // Generate the kerning table
    for (int i = 0; i < NCHAR; i++)
        for (int j = 0; j < NCHAR; j++) {
            FT_Vector offset;
            int status = FT_Get_Kerning(face,
                charMap.chars[i].ftIdx, charMap.chars[j].ftIdx,
                FT_KERNING_UNFITTED, &offset);
            assert(!status);
            charMap.kerning[i * NCHAR + j] = offset.x / 64;
        }
}

void Font::createCharacterAtlas(FT_Face face) {
    charMap.atlasWidth = charMap.maxWidth * CHAR_DIM;
    charMap.atlasHeight = charMap.maxHeight * CHAR_DIM;

    char *pixels = (char *)malloc(charMap.atlasWidth * charMap.atlasHeight);
    memset(pixels, 0, charMap.atlasWidth * charMap.atlasHeight);

    // Generate all the character bitmaps
    for (int y = 0; y < CHAR_DIM; y++)
        for (int x = 0; x < CHAR_DIM; x++) {
            int idx = y * CHAR_DIM + x;

            if (idx >= NCHAR)
                break;

            int ftIdx = charMap.chars[idx].ftIdx;

            int status = FT_Load_Glyph(face, ftIdx, FT_LOAD_RENDER);
            assert(!status);

            int y0 = y * charMap.maxHeight;
            int x0 = x * charMap.maxWidth;

            // Copy bitmap pixels into atlas
            for (int j = 0; j < charMap.chars[idx].height; j++)
                for (int k = 0; k < charMap.chars[idx].width; k++) {
                    int src = j * charMap.chars[idx].width + k;
                    int dst = (y0 + j) * charMap.atlasWidth + x0 + k;

                    pixels[dst] = face->glyph->bitmap.buffer[src];
                }
        }

    assert(glGetError() == GL_NO_ERROR);

    // Create GL texture for atlas
    glGenTextures(1, &atlas);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, charMap.atlasWidth,
        charMap.atlasHeight);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, charMap.atlasWidth,
        charMap.atlasHeight, GL_RED, GL_UNSIGNED_BYTE, pixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // TODO is this necessary
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    assert(glGetError() == GL_NO_ERROR);

    free(pixels);
}

// TODO: once graphics abstraction is "done", move GL headers from defs.h

void Font::flush(int screenW, int screenH) {
    buff->flush(screenW, screenH);
}

void Font::drawString(const char *string, int x, int y) {
    int chLast;
    bool offset = false;

    int x0 = x;

    for (int i = 0; string[i] != 0; i++) {
        int chIdx = (int)string[i] - MIN_CHAR;

        if (string[i] == '\n') {
            x = x0;
            y += charMap.maxHeight + lineHeight;
            offset = false;
            continue;
        }
        else if (offset)
            x += charMap.chars[chLast].advance +
                charMap.kerning[chLast * NCHAR + chIdx];

        offset = true;

        int cy = chIdx / CHAR_DIM;
        int cx = chIdx % CHAR_DIM;

        CharInfo *ch = &charMap.chars[chIdx];

        float fu_0 = cx * charMap.maxWidth;
        float fu_1 = cx * charMap.maxWidth + ch->width;
        float fv_0 = cy * charMap.maxHeight;
        float fv_1 = cy * charMap.maxHeight + ch->height;

        float su_0 = x + ch->left;
        float su_1 = x + ch->left + ch->width;
        float sv_0 = y + charMap.maxHeight - ch->top;
        float sv_1 = y + ch->height + charMap.maxHeight - ch->top;

        int i0 = buff->numVertices();

        buff->addVertex(
            glm::vec2(su_0, sv_0),
            glm::vec2(fu_0, fv_0),
            color);

        buff->addVertex(
            glm::vec2(su_0, sv_1),
            glm::vec2(fu_0, fv_1),
            color);

        buff->addVertex(
            glm::vec2(su_1, sv_1),
            glm::vec2(fu_1, fv_1),
            color);

        buff->addVertex(
            glm::vec2(su_1, sv_0),
            glm::vec2(fu_1, fv_0),
            color);

        buff->addIndex(i0 + 2);
        buff->addIndex(i0 + 1);
        buff->addIndex(i0 + 0);
        buff->addIndex(i0 + 3);
        buff->addIndex(i0 + 2);
        buff->addIndex(i0 + 0);

        chLast = chIdx;
    }
}

void Font::setColor(glm::vec4 color) {
    this->color = color;
}

}
