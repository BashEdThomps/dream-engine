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
namespace Dream
{
    void Constants::dreamSetDebug(bool debug)
    {
        cout << "===== DEBUG: " << (debug? "Enabled" : "Disabled") << " =====" << endl;
        DEBUG = debug;
    }

    void Constants::dreamSetVerbose(bool verbose)
    {
        cout << "===== VERBOSE: " << (verbose? "Enabled" : "Disabled") << " =====" << endl;
        VERBOSE = verbose;
        if (VERBOSE)
        {
            dreamSetDebug(true);
        }
    }

    // Misc ========================================================================
    const string Constants::DIR_PATH_SEP = "/";
    const string Constants::DREAM_TOOL_DIR = ".dreamtool";
    const string Constants::PATH_DELIMETER = "::";
    const string Constants::HTTP_ARG = "--http";

    // Project =====================================================================
    const string Constants::PROJECT_DIRECTORY_ARG = "--project-dir";
    const string Constants::PROJECT_UUID_ARG = "--uuid";
    const int    Constants::PROJECT_UUID_LENGTH = 19;
    const string Constants::PROJECT_EXTENSION = ".json";
    const string Constants::PROJECT_PATH_SEP = "/";
    const string Constants::PROJECT_UUID = "uuid";
    const string Constants::PROJECT_NAME = "name";
    const string Constants::PROJECT_AUTHOR = "author";
    const string Constants::PROJECT_DESCRIPTION = "description";
    const string Constants::PROJECT_SCENE_ARRAY = "scenes";
    const string Constants::PROJECT_ASSET_ARRAY = "assets";
    const string Constants::PROJECT_STARTUP_SCENE = "startupScene";
    const string Constants::PROJECT_WINDOW_SIZE  = "windowSize";
    const string Constants::PROJECT_WIDTH = "width";
    const string Constants::PROJECT_HEIGHT = "height";

    // Asset =======================================================================
    const string Constants::ASSET_UUID = "uuid";
    const string Constants::ASSET_NAME = "name";
    const string Constants::ASSET_TYPE = "type";
    const string Constants::ASSET_DIR = "assets";
    const string Constants::ASSET_TYPE_ANIMATION = "animation";
    const string Constants::ASSET_TYPE_AUDIO = "audio";
    const string Constants::ASSET_TYPE_LIGHT = "light";
    const string Constants::ASSET_TYPE_FONT = "font";
    const string Constants::ASSET_TYPE_MODEL = "model";
    const string Constants::ASSET_TYPE_PHYSICS_OBJECT = "physicsObject";
    const string Constants::ASSET_TYPE_SCRIPT = "script";
    const string Constants::ASSET_TYPE_SHADER = "shader";
    const string Constants::ASSET_TYPE_SPRITE = "sprite";
    const string Constants::ASSET_FORMAT = "format";
    const string Constants::ASSET_FORMAT_SCRIPT_LUA = "lua";
    const string Constants::ASSET_FORMAT_MODEL_ASSIMP = "assimp";
    const string Constants::ASSET_FORMAT_AUDIO_MUSIC = "music";
    const string Constants::ASSET_FORMAT_AUDIO_SFX = "sfx";
    const string Constants::ASSET_FORMAT_ANIMATION_DREAM = "dream";
    const string Constants::ASSET_FORMAT_FONT_TTF = "ttf";
    const string Constants::ASSET_FORMAT_SHADER_GLSL = "glsl";
    const string Constants::ASSET_FORMAT_LIGHT_POINT = "point";
    const string Constants::ASSET_FORMAT_SPRITE_IMAGE = "image";
    const string Constants::ASSET_ATTR_X = "x";
    const string Constants::ASSET_ATTR_Y = "y";
    const string Constants::ASSET_ATTR_Z = "z";

    // Scene =======================================================================
    const string Constants::SCENE_JSON_UUID = "uuid";
    const string Constants::SCENE_JSON_NAME = "name";
    const string Constants::SCENE_JSON_NOTES = "notes";
    const string Constants::SCENE_JSON_SCENE_OBJECTS = "objects";
    const string Constants::SCENE_JSON_CAMERA = "camera";
    const string Constants::SCENE_JSON_TRANSLATION = "translation";
    const string Constants::SCENE_JSON_ROTATION = "rotation";
    const string Constants::SCENE_JSON_MOVEMENT_SPEED = "movementSpeed";
    const string Constants::SCENE_JSON_X = "x";
    const string Constants::SCENE_JSON_Y = "y";
    const string Constants::SCENE_JSON_Z = "z";
    const string Constants::SCENE_JSON_CLEAR_COLOUR = "clearColour";
    const string Constants::SCENE_JSON_AMBIENT_LIGHT_COLOUR = "ambientLight";
    const string Constants::SCENE_JSON_RED = "r";
    const string Constants::SCENE_JSON_GREEN = "g";
    const string Constants::SCENE_JSON_BLUE = "b";
    const string Constants::SCENE_JSON_ALPHA = "a";
    const string Constants::SCENE_JSON_GRAVITY = "gravity";
    const string Constants::SCENE_JSON_PHYSICS_DEBUG = "physicsDebug";

    // SceneObject =================================================================
    const int Constants::SO_X = 0;
    const int Constants::SO_Y = 1;
    const int Constants::SO_Z = 2;
    const string Constants::SCENE_OBJECT_UUID = "uuid";
    const string Constants::SCENE_OBJECT_NAME = "name";
    const string Constants::SCENE_OBJECT_CHILDREN = "children";
    const string Constants::SCENE_OBJECT_HAS_FOCUS = "hasFocus";
    const string Constants::SCENE_OBJECT_TRANSLATION = "translation";
    const string Constants::SCENE_OBJECT_ROTATION = "rotation";
    const string Constants::SCENE_OBJECT_SCALE = "scale";
    const string Constants::SCENE_OBJECT_X = "x";
    const string Constants::SCENE_OBJECT_Y = "y";
    const string Constants::SCENE_OBJECT_Z = "z";
    const string Constants::SCENE_OBJECT_TRANSFORM_TYPE = "transformType";
    const string Constants::SCENE_OBJECT_TRANSFORM_TYPE_OFFSET = "offset";
    const string Constants::SCENE_OBJECT_TRANSFORM_TYPE_ABSOLUTE = "absolute";
    const string Constants::SCENE_OBJECT_ASSET_INSTANCES = "assetInstances";

    // Transform ===================================================================
    const int Constants::TRANSFORM_X = 0;
    const int Constants::TRANSFORM_Y = 1;
    const int Constants::TRANSFORM_Z = 2;
    const string Constants::TRANSFORM_TYPE_ABSOLUTE = "absolute";
    const string Constants::TRANSFORM_TYPE_OFFSET = "offset";

    // Font ========================================================================
    const string Constants::FONT_SIZE = "size";
    const string Constants::FONT_COLOUR = "colour";
    const string Constants::FONT_RED   = "r";
    const string Constants::FONT_GREEN = "g";
    const string Constants::FONT_BLUE  = "b";

    // Animation ===================================================================
    const string Constants::ASSET_ATTR_KEYFRAMES     = "keyframes";
    const string Constants::ASSET_ATTR_TRANSLATION   = "translation";
    const string Constants::ASSET_ATTR_ROTATION      = "rotation";
    const string Constants::ASSET_ATTR_SCALE         = "scale";
    const string Constants::ASSET_ATTR_START_TIME    = "startTime";
    const string Constants::ASSET_ATTR_WRAP          = "wrap";
    const string Constants::ASSET_ATTR_LOOP          = "loop";
    const string Constants::ASSET_ATTR_INTERPOLATION = "interpolation";
    const int Constants::DREAM_ANIMATION_STOPPED = 0;
    const int Constants::DREAM_ANIMATION_PLAYING = 1;
    const int Constants::DREAM_ANIMATION_PAUSED  = 2;
    const int Constants::MAX_FRAME_ADVANCE       = 10;
    const string Constants::DREAM_ANIMATION_INTERPOLATION_NONE   = "none";
    const string Constants::DREAM_ANIMATION_INTERPOLATION_LINEAR = "linear";
    const string Constants::DREAM_ANIMATION_INTERPOLATION_BEZIER = "bezier";

    // Sprite ======================================================================
    const string Constants::SPRITE_TILE_SIZE   = "tileSize";
    const string Constants::SPRITE_TILE_WIDTH  = "width";
    const string Constants::SPRITE_TILE_HEIGHT = "height";

    // Physics =====================================================================
    const string Constants::ASSET_ATTR_MASS   = "mass";
    const string Constants::ASSET_ATTR_MARGIN = "margin";
    const string Constants::ASSET_ATTR_RADIUS = "radius";
    const string Constants::ASSET_ATTR_NORMAL = "normal";
    const string Constants::ASSET_ATTR_CONSTANT = "const ant";
    const string Constants::ASSET_ATTR_NORMAL_X = "normalX";
    const string Constants::ASSET_ATTR_NORMAL_Y = "normalY";
    const string Constants::ASSET_ATTR_NORMAL_Z = "normalZ";
    const string Constants::ASSET_ATTR_SIZE = "size";
    const string Constants::ASSET_ATTR_SIZE_X = "sizeX";
    const string Constants::ASSET_ATTR_SIZE_Y = "sizeY";
    const string Constants::ASSET_ATTR_SIZE_Z = "sizeZ";
    const string Constants::ASSET_ATTR_COMPOUND_CHILDREN = "compoundChildren";
    const string Constants::ASSET_ATTR_KINEMATIC = "kinematic";
    const string Constants::COLLISION_SHAPE_SPHERE = "btShpereShape";
    const string Constants::COLLISION_SHAPE_BOX = "btBoxShape";
    const string Constants::COLLISION_SHAPE_CYLINDER = "btCylinderShape";
    const string Constants::COLLISION_SHAPE_CAPSULE = "btCapsuleShape";
    const string Constants::COLLISION_SHAPE_CONE = "btConeShape";
    const string Constants::COLLISION_SHAPE_MULTI_SPHERE = "btMultiSphereShape";
    const string Constants::COLLISION_SHAPE_CONVEX_HULL = "btConvexHullShape";
    const string Constants::COLLISION_SHAPE_CONVEX_TRIANGLE_MESH = "btConvexTriangleMeshShape";
    const string Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH = "btBvhTriangleMeshShape";
    const string Constants::COLLISION_SHAPE_HEIGHTFIELD_TERRAIN = "btHeightfieldTerrainShape";
    const string Constants::COLLISION_SHAPE_STATIC_PLANE = "btStaticPlaneShape";
    const string Constants::COLLISION_SHAPE_COMPOUND = "btCompoundShape";

    // Event =======================================================================
    const string Constants::EVENT_SENDER = "sender";
    const string Constants::EVENT_TYPE = "type";
    const string Constants::EVENT_TYPE_COLLISION = "collision";

    // Light =======================================================================
    const string Constants::ASSET_ATTR_LIGHT_COLOR   = "colour";
    const string Constants::ASSET_ATTR_LIGHT_COLOR_R = "r";
    const string Constants::ASSET_ATTR_LIGHT_COLOR_G = "g";
    const string Constants::ASSET_ATTR_LIGHT_COLOR_B = "b";
    const string Constants::ASSET_ATTR_LIGHT_COLOR_A = "a";

    map<AssetType,string> Constants::DREAM_ASSET_TYPES_MAP =
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

    map<AssetType,vector<string>> Constants::DREAM_ASSET_FORMATS_MAP =
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

    string Constants::assetTypeToString(AssetType type)
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

    AssetType Constants::assetTypeFromString(string type)
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

} // End of Dream
