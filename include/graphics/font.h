/**
 * @file font.h
 *
 * @brief Font rendering utility class
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

#ifndef __FONT_H
#define __FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

#define MIN_CHAR 32                       //!< Minimum character index (' ')
#define MAX_CHAR 126                      //!< Maximum character index ('~')
#define NCHAR (MAX_CHAR - MIN_CHAR + 1)   //!< Number of characters
#define CHAR_DIM 10                       //!< Chars per row/column in atlas

namespace Graphics {

class Mesh;
class MeshVertex;
class Shader;

// TODO:
//   - Could double buffer quads if updating them every frame becomes a
//     bottleneck
//   - May want to create atlas offline
//   - Handle varying DPI
//   - Related, probably want to position/size characters in a
//     resolution-independent way
//   - Maybe use shared pointers

class FontBuffer {
public:

    virtual ~FontBuffer() = 0;

    virtual void setup(int atlasWidth, int atlasHeight, GLuint atlasTexture) = 0;

    // TODO: I don't like the virtualness ;(
    virtual void addVertex(glm::vec2 position, glm::vec2 uv, glm::vec4 color) = 0;

    virtual void addIndex(int index) = 0;

    virtual void flush(int screenW, int screenH) = 0;

    virtual int numVertices() = 0;

    virtual int numIndices() = 0;

};

// TODO document, possible merge back into Font
// TODO unsigned indices
class UtilFontBuffer : public FontBuffer {
private:

    Mesh                       *mesh;         //!< Font vertices/indices
    Shader                     *shader;       //!< Character shader
    std::vector<MeshVertex>     vertices;     //!< Buffered character quad vertices
    std::vector<int>            indices;      //!< Buffered character quad indices
    GLuint                      atlas;        //!< Font character atlas texture

public:

    UtilFontBuffer();

    virtual ~UtilFontBuffer() override;

    virtual void setup(int atlasWidth, int atlasHeight, GLuint atlasTexture) override;

    virtual void addVertex(glm::vec2 position, glm::vec2 uv, glm::vec4 color) override;

    virtual void addIndex(int index) override;

    virtual void flush(int screenW, int screenH) override;

    virtual int numVertices() override;

    virtual int numIndices() override;

    // TODO check destructor

};

/**
 * @brief Font rendering utility class
 */
class Font {
private:

    /**
     * @brief Information about a specific character in the character map
     */
    struct CharInfo {
        int ftIdx;                        //!< Freetype character index
        int width;                        //!< Bitmap width
        int height;                       //!< Bitmap height
        int left;                         //!< Distance to bitmap left
        int top;                          //!< Distance to bitmap top
        int advance;                      //!< Distance to offset X from origin
    };

    /**
     * @brief Information about characters. Sizes are in pixels. This table is
     * populated from FreeType. Actual images are stored in a texture atlas.
     */
    struct CharMap {
        int      maxWidth;                //!< Width of largest character
        int      maxHeight;               //!< Height of largest character
        int      atlasWidth;              //!< Texture atlas width
        int      atlasHeight;             //!< Texture atlas height
        CharInfo chars[NCHAR];            //!< Per-character information
        int      kerning[NCHAR * NCHAR];  //!< Kerning table
    };

    static bool             ftInitted;    //!< Whether freetype was initialized
    static FT_Library       library;      //!< Freetype

    int                     lineHeight;   //!< Additional pixels between lines
    GLuint                  atlas;        //!< Character texture atlas
    glm::vec4               color;        //!< Text color
    CharMap                 charMap;      //!< Character information

    FontBuffer             *buff;         //!< Font buffer
    bool                    ownBuff;      //!< Whether we own the buffer

    /**
     * @brief Fill the character map from at FT_Face
     *
     * @param[in] face Font face to generate map from
     */
    void createCharacterMap(FT_Face face);

    /**
     * @brief Generate the character texture atlas
     *
     * @param[in] face Font face to generate texture from
     */
    void createCharacterAtlas(FT_Face face);

public:

    /**
     * @brief Constructor
     *
     * @param[in] filename   Font filename
     * @param[in] faceIdx    Face index within font
     * @param[in] ptSize     Size of font in points
     * @param[in] lineHeight Additional space between lines
     */
    Font(std::string filename, int faceIdx, int ptSize, int lineHeight = 0, FontBuffer *buff = NULL);

    /**
     * @brief Destructor
     */
    ~Font();

    /**
     * @brief Add a string to the character buffer. It will not actually be
     *        displayed until flush() is called.
     *
     * @param[in] string String to add to buffer
     * @param[in] x      Upper-left X coordinate in pixels, relative to top-left
     *                   corner of screen
     * @param[in] y      Upper-left Y coordinate in pixels, relative to top-left
     *                   corner of screen
     */
    void drawString(const char *string, int x, int y);

    /**
     * @brief Draw all buffered strings. They will be drawn using the current
     *        color.
     *
     * @param[in] screenW Screen width
     * @param[in] screenH Screen height
     */
    void flush(int screenW, int screenH);

    /**
     * @brief Set the color to use for the next flush() call
     *
     * @param[in] color New color
     */
    void setColor(glm::vec4 color);

};

}

#endif
