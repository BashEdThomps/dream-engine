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

#pragma once

#include <string>
#include <memory>
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../IAssetInstance.h"

using std::string;
using nlohmann::json;
using glm::vec3;

namespace Dream
{
    class FontCache;
    class FontCharacter;
    class FontDefinition;

    class FontInstance : public IAssetInstance
    {
    private:
        int mSize;
        shared_ptr<FontCache> mCache;
        shared_ptr<FT_Face> mFontFace;
        vector<float> mColour;
        string mText;
    private: // Methods
        void generateCharacterMap();
    public:
        FontInstance(
            const shared_ptr<FontCache>&,
            const shared_ptr<FontDefinition>&,
            const shared_ptr<SceneObjectRuntime>&
        );

        ~FontInstance();

        bool load(string);
        void loadExtraAttributes(json);

        shared_ptr<FT_Face> getFontFace();

        void setText(string);
        string getText();

        void setColour(float,float,float);
        vector<float> getColour();
        vec3 getColourAsVec3();

        void setSize(int);
        int getWidth();
        int getHeight();

        map<GLchar,FontCharacter> getCharMap();

    }; // End of FontInstance

} // End of Dream
