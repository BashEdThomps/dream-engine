/*
 * Constants
 *
 * Created: 01/12/2016 2016 by Ashley
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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace Dream
{
    enum AssetType
    {
        ANIMATION,
        AUDIO,
        FONT,
        LIGHT,
        MODEL,
        PHYSICS_OBJECT,
        SCRIPT,
        SHADER,
        SPRITE,
        NONE
    };

    class Constants
    {
    public:
        // Debug =======================================================================
        static bool DEBUG;
        static bool VERBOSE;

        static void dreamSetDebug(bool debug);
        static void dreamSetVerbose(bool verbose);
        static bool checkGLError(string);

        // Misc ========================================================================
        const static string DIR_PATH_SEP;
        const static string DREAM_TOOL_DIR;
        const static string PATH_DELIMETER;
        const static string HTTP_ARG;

        // Project =====================================================================
        const static string PROJECT_DIRECTORY_ARG;
        const static string PROJECT_UUID_ARG;
        const static int    PROJECT_UUID_LENGTH;
        const static string PROJECT_EXTENSION;
        const static string PROJECT_PATH_SEP;
        const static string PROJECT_UUID;
        const static string PROJECT_NAME;
        const static string PROJECT_AUTHOR;
        const static string PROJECT_DESCRIPTION;
        const static string PROJECT_SCENE_ARRAY;
        const static string PROJECT_ASSET_ARRAY;
        const static string PROJECT_STARTUP_SCENE;
        const static string PROJECT_WINDOW_SIZE;
        const static string PROJECT_WIDTH;
        const static string PROJECT_HEIGHT;

        // Asset =======================================================================
        const static string ASSET_UUID;
        const static string ASSET_NAME;
        const static string ASSET_TYPE;
        const static string ASSET_DIR;
        const static string ASSET_TYPE_ANIMATION;
        const static string ASSET_TYPE_AUDIO;
        const static string ASSET_TYPE_LIGHT;
        const static string ASSET_TYPE_FONT;
        const static string ASSET_TYPE_MODEL;
        const static string ASSET_TYPE_PHYSICS_OBJECT;
        const static string ASSET_TYPE_SCRIPT;
        const static string ASSET_TYPE_SHADER;
        const static string ASSET_TYPE_SPRITE;
        const static string ASSET_FORMAT;
        const static string ASSET_FORMAT_SCRIPT_LUA;
        const static string ASSET_FORMAT_MODEL_ASSIMP;
        const static string ASSET_FORMAT_AUDIO_MUSIC;
        const static string ASSET_FORMAT_AUDIO_SFX;
        const static string ASSET_FORMAT_ANIMATION_DREAM;
        const static string ASSET_FORMAT_FONT_TTF;
        const static string ASSET_FORMAT_SHADER_GLSL;
        const static string ASSET_FORMAT_LIGHT_POINT;
        const static string ASSET_FORMAT_SPRITE_IMAGE;
        const static string ASSET_ATTR_X;
        const static string ASSET_ATTR_Y;
        const static string ASSET_ATTR_Z;

        // Scene =======================================================================
        const static string SCENE_JSON_UUID;
        const static string SCENE_JSON_NAME;
        const static string SCENE_JSON_NOTES;
        const static string SCENE_JSON_SCENE_OBJECTS;
        const static string SCENE_JSON_CAMERA;
        const static string SCENE_JSON_TRANSLATION;
        const static string SCENE_JSON_ROTATION;
        const static string SCENE_JSON_MOVEMENT_SPEED;
        const static string SCENE_JSON_X;
        const static string SCENE_JSON_Y;
        const static string SCENE_JSON_Z;
        const static string SCENE_JSON_CLEAR_COLOUR;
        const static string SCENE_JSON_AMBIENT_LIGHT_COLOUR;
        const static string SCENE_JSON_RED;
        const static string SCENE_JSON_GREEN;
        const static string SCENE_JSON_BLUE;
        const static string SCENE_JSON_ALPHA;
        const static string SCENE_JSON_GRAVITY;
        const static string SCENE_JSON_PHYSICS_DEBUG;

        // SceneObject =================================================================
        const static int SO_X;
        const static int SO_Y;
        const static int SO_Z;
        const static string SCENE_OBJECT_UUID;
        const static string SCENE_OBJECT_NAME;
        const static string SCENE_OBJECT_CHILDREN;
        const static string SCENE_OBJECT_HAS_FOCUS;
        const static string SCENE_OBJECT_TRANSLATION;
        const static string SCENE_OBJECT_ROTATION;
        const static string SCENE_OBJECT_SCALE;
        const static string SCENE_OBJECT_X;
        const static string SCENE_OBJECT_Y;
        const static string SCENE_OBJECT_Z;
        const static string SCENE_OBJECT_TRANSFORM_TYPE;
        const static string SCENE_OBJECT_TRANSFORM_TYPE_OFFSET;
        const static string SCENE_OBJECT_TRANSFORM_TYPE_ABSOLUTE;
        const static string SCENE_OBJECT_ASSET_INSTANCES;

        // Transform ===================================================================
        const static int TRANSFORM_X;
        const static int TRANSFORM_Y;
        const static int TRANSFORM_Z;
        const static string TRANSFORM_TYPE_ABSOLUTE;
        const static string TRANSFORM_TYPE_OFFSET;

        // Font ========================================================================
        const static string FONT_SIZE;
        const static string FONT_COLOUR;
        const static string FONT_RED;
        const static string FONT_GREEN;
        const static string FONT_BLUE;

        // Animation ===================================================================
        const static string ASSET_ATTR_KEYFRAMES;
        const static string ASSET_ATTR_TRANSLATION;
        const static string ASSET_ATTR_ROTATION;
        const static string ASSET_ATTR_SCALE;
        const static string ASSET_ATTR_START_TIME;
        const static string ASSET_ATTR_WRAP;
        const static string ASSET_ATTR_LOOP;
        const static string ASSET_ATTR_INTERPOLATION;
        const static int DREAM_ANIMATION_STOPPED;
        const static int DREAM_ANIMATION_PLAYING;
        const static int DREAM_ANIMATION_PAUSED;
        const static int MAX_FRAME_ADVANCE;
        const static string DREAM_ANIMATION_INTERPOLATION_NONE;
        const static string DREAM_ANIMATION_INTERPOLATION_LINEAR;
        const static string DREAM_ANIMATION_INTERPOLATION_BEZIER;

        // Sprite ======================================================================
        const static string SPRITE_TILE_SIZE;
        const static string SPRITE_TILE_WIDTH;
        const static string SPRITE_TILE_HEIGHT;

        // Physics =====================================================================
        const static string ASSET_ATTR_MASS;
        const static string ASSET_ATTR_MARGIN;
        const static string ASSET_ATTR_RADIUS;
        const static string ASSET_ATTR_NORMAL;
        const static string ASSET_ATTR_CONSTANT;
        const static string ASSET_ATTR_NORMAL_X;
        const static string ASSET_ATTR_NORMAL_Y;
        const static string ASSET_ATTR_NORMAL_Z;
        const static string ASSET_ATTR_SIZE;
        const static string ASSET_ATTR_SIZE_X;
        const static string ASSET_ATTR_SIZE_Y;
        const static string ASSET_ATTR_SIZE_Z;
        const static string ASSET_ATTR_COMPOUND_CHILDREN;
        const static string ASSET_ATTR_KINEMATIC;
        const static string COLLISION_SHAPE_SPHERE;
        const static string COLLISION_SHAPE_BOX;
        const static string COLLISION_SHAPE_CYLINDER;
        const static string COLLISION_SHAPE_CAPSULE;
        const static string COLLISION_SHAPE_CONE;
        const static string COLLISION_SHAPE_MULTI_SPHERE;
        const static string COLLISION_SHAPE_CONVEX_HULL;
        const static string COLLISION_SHAPE_CONVEX_TRIANGLE_MESH;
        const static string COLLISION_SHAPE_BVH_TRIANGLE_MESH;
        const static string COLLISION_SHAPE_HEIGHTFIELD_TERRAIN;
        const static string COLLISION_SHAPE_STATIC_PLANE;
        const static string COLLISION_SHAPE_COMPOUND;

        // Event =======================================================================
        const static string EVENT_SENDER;
        const static string EVENT_TYPE;
        const static string EVENT_TYPE_COLLISION;

        // Light =======================================================================
        const static string ASSET_ATTR_LIGHT_COLOR;
        const static string ASSET_ATTR_LIGHT_COLOR_R;
        const static string ASSET_ATTR_LIGHT_COLOR_G;
        const static string ASSET_ATTR_LIGHT_COLOR_B;
        const static string ASSET_ATTR_LIGHT_COLOR_A;

        static map<AssetType,string> DREAM_ASSET_TYPES_MAP;
        static map<AssetType,vector<string>> DREAM_ASSET_FORMATS_MAP;
        static string assetTypeToString(AssetType type);
        static AssetType assetTypeFromString(string type);

    };
}

#endif // CONSTANTS_H
