/*
 * FontCharacter.h
 *
 * Created: 12 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"

using glm::ivec2;
using glm::vec3;
using glm::vec2;

namespace Dream
{
    struct FontCharacterVertex
    {
        vec3 Translation;
        vec2 TextureCoordinates;

        FontCharacterVertex(vec3 translation, vec2 coords)
            : Translation(translation), TextureCoordinates(coords){}
    };

    class FontCharacter
    {
    public:
        FontCharacter();
        FontCharacter(GLuint textureId, ivec2 size, ivec2 bearing, GLuint advance);
        FontCharacter(const FontCharacter&);
        ~FontCharacter();

        GLuint TextureID; // ID handle of the glyph texture
        ivec2  Size;      // Size of glyph
        ivec2  Bearing;   // Offset from baseline to left/top of glyph
        GLuint Advance;   // Offset to advance to next glyph

    };
}
