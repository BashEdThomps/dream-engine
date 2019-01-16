/*
 * TextureManager.h
 *
 * Created: 30/11/2016 2016 by Ashley
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

#include "../../Cache.h"

namespace Dream
{
    class ShaderCache;
    class TextureCache;

    class MaterialCache : public Cache
    {
        ShaderCache* mShaderCache;
        TextureCache* mTextureCache;
    public:
        MaterialCache(ProjectRuntime*, ShaderCache*, TextureCache*);
        ~MaterialCache() override;

    protected:
        SharedAssetRuntime* loadRuntime(AssetDefinition* def) override;
    };

} // End Dream

