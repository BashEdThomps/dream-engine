/*
 * Log.cpp
 *
 * Created: 05/04/2017 2017 by Ashley
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

#include "Constants.h"

using namespace std;

bool DEBUG(true);
bool VERBOSE(false);

void dreamSetDebug(bool debug)
{
    cout << "===== DEBUG: " << (debug? "Enabled" : "Disabled") << " =====" << endl;
    DEBUG = debug;
}

void dreamSetVerbose(bool verbose)
{
    cout << "===== VERBOSE: " << (verbose? "Enabled" : "Disabled") << " =====" << endl;
    VERBOSE = verbose;
    if (VERBOSE)
    {
        dreamSetDebug(true);
    }
}

namespace Dream
{
    map<AssetType,string> DREAM_ASSET_TYPES_MAP =
    {
        {
            AssetType::ANIMATION, ASSET_TYPE_ANIMATION
        },
        {
            AssetType::AUDIO, ASSET_TYPE_AUDIO
        },
        {
            AssetType::FONT, ASSET_TYPE_FONT
        },
        {
            AssetType::LIGHT, ASSET_TYPE_LIGHT
        },
        {
            AssetType::MODEL, ASSET_TYPE_MODEL
        },
        {
            AssetType::PHYSICS_OBJECT, ASSET_TYPE_PHYSICS_OBJECT
        },
        {
            AssetType::SCRIPT, ASSET_TYPE_SCRIPT
        },
        {
            AssetType::SHADER, ASSET_TYPE_SHADER
        },
        {
            AssetType::SPRITE, ASSET_TYPE_SPRITE
        }
    };

    map<AssetType,vector<string>> DREAM_ASSET_FORMATS_MAP =
    {
        {
            AssetType::ANIMATION,
            {
                ASSET_FORMAT_ANIMATION_DREAM
            }
        },
        {
            AssetType::AUDIO,
            {
                ASSET_FORMAT_AUDIO_MUSIC,
                ASSET_FORMAT_AUDIO_SFX
            }
        },
        {
            AssetType::FONT,
            {
                ASSET_FORMAT_FONT_TTF
            }
        },
        {
            AssetType::LIGHT,
            {
                ASSET_FORMAT_LIGHT_POINT
            }
        },
        {
            AssetType::MODEL,
            {
                ASSET_FORMAT_MODEL_ASSIMP
            }
        },
        {
            AssetType::PHYSICS_OBJECT,
            {
                COLLISION_SHAPE_SPHERE,
                COLLISION_SHAPE_BOX,
                COLLISION_SHAPE_CYLINDER,
                COLLISION_SHAPE_CAPSULE,
                COLLISION_SHAPE_CONE,
                COLLISION_SHAPE_MULTI_SPHERE,
                COLLISION_SHAPE_CONVEX_HULL,
                COLLISION_SHAPE_CONVEX_TRIANGLE_MESH,
                COLLISION_SHAPE_BVH_TRIANGLE_MESH,
                COLLISION_SHAPE_HEIGHTFIELD_TERRAIN,
                COLLISION_SHAPE_STATIC_PLANE,
                COLLISION_SHAPE_COMPOUND
            }
        },
        {
            AssetType::SCRIPT,
            {
                ASSET_FORMAT_SCRIPT_LUA
            }
        },
        {
            AssetType::SHADER,
            {
                ASSET_FORMAT_SHADER_GLSL
            }
        },
        {
            AssetType::SPRITE,
            {
                ASSET_FORMAT_SPRITE_IMAGE
            }
        }
    };

    string assetTypeToString(AssetType type)
    {
        switch (type)
        {
            case ANIMATION:
                return ASSET_TYPE_ANIMATION;
            case AUDIO:
                return ASSET_TYPE_AUDIO;
            case FONT:
                return ASSET_TYPE_FONT;
            case LIGHT:
                return ASSET_TYPE_LIGHT;
            case MODEL:
                return ASSET_TYPE_MODEL;
            case PHYSICS_OBJECT:
                return ASSET_TYPE_PHYSICS_OBJECT;
            case SCRIPT:
                return ASSET_TYPE_SCRIPT;
            case SHADER:
                return ASSET_TYPE_SHADER;
            case SPRITE:
                return ASSET_TYPE_SPRITE;
            case NONE:
                return "";
        }
        return "";
    }

    AssetType assetTypeFromString(string type)
    {
        if (type.compare(ASSET_TYPE_ANIMATION) == 0)
        {
            return ANIMATION;
        }
        else if (type.compare(ASSET_TYPE_AUDIO) == 0)
        {
            return AUDIO;
        }
        else if (type.compare(ASSET_TYPE_FONT) == 0)
        {
            return FONT;
        }
        else if (type.compare(ASSET_TYPE_LIGHT) == 0)
        {
            return LIGHT;
        }
        else if (type.compare(ASSET_TYPE_MODEL) == 0)
        {
            return MODEL;
        }
        else if (type.compare(ASSET_TYPE_PHYSICS_OBJECT) == 0)
        {
            return PHYSICS_OBJECT;
        }
        else if (type.compare(ASSET_TYPE_SCRIPT) == 0)
        {
            return SCRIPT;
        }
        else if (type.compare(ASSET_TYPE_SHADER) == 0)
        {
            return SHADER;
        }
        else if (type.compare(ASSET_TYPE_SPRITE) == 0)
        {
            return SPRITE;
        }
        return NONE;
    }
};
