/*
 * FontInstance
 *
 * Copyright 2016 Octronic. All rights reserved.
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

#ifndef FONTINSTANCE_H
#define FONTINSTANCE_H

#include <memory>
#include <GL/glew.h>
#include "../../IAssetInstance.h"
#include "../../Constants.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

namespace Dream
{
    struct Character
    {
        GLuint     TextureID;  // ID handle of the glyph texture
        glm::ivec2 Size;       // Size of glyph
        glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
        GLuint     Advance;    // Offset to advance to next glyph
    };

    class FontInstance : public IAssetInstance
    {
    private:
        map<GLchar, Character> mCharacterMap;
        static unique_ptr<FT_Library> sFreeTypeLib;
        int mSize;
        unique_ptr<FT_Face> mFontFace;
        vector<float> mColour;
        string mText;
    private: // Methods
        void ftInit();
        void ftDestroy();
        void generateCharacterMap();
    public:
        FontInstance(AssetDefinition*,Transform3D*);
        ~FontInstance();

        bool load(string);
        void loadExtraAttributes(nlohmann::json);

        FT_Face* getFontFace();

        void setText(string);
        string getText();

        void setColour(float,float,float);
        vector<float> getColour();

        void setSize(int);
        int getWidth();
        int getHeight();
        map<GLchar, Character> getCharacterMap();

    }; // End of FontInstance
} // End of Dream

#endif // FONTINSTANCE_H
