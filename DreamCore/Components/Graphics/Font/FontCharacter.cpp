/*
 * FontCharacter.cpp
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
#include "FontCharacter.h"

namespace Dream
{
    FontCharacter::FontCharacter
    ()
    {

    }

    FontCharacter::FontCharacter
    (GLuint textureId, ivec2 size, ivec2 bearing, GLuint advance)
    : TextureID(textureId),
      Size(size),
      Bearing(bearing),
      Advance(advance)
    {

    }

    FontCharacter::FontCharacter(const FontCharacter &that)
    {
        this->TextureID  = that.TextureID;
        this->Size = that.Size;
        this->Bearing = that.Bearing;
        this->Advance = that.Advance;
    }

    FontCharacter::~FontCharacter()
    {
        /*
        if (TextureID != -1)
        {
            glDeleteTextures(1,&TextureID);
        }
        */
    }

} // End Dream
