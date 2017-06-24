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

#pragma once

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
        // Debug ================================================================
        static bool DEBUG;
        static bool VERBOSE;
        static void dreamSetDebug(bool debug);
        static void dreamSetVerbose(bool verbose);
        static bool checkGLError(string);
        // Misc =================================================================
        const static string DIR_PATH_SEP;
        const static string DREAM_TOOL_DIR;
        const static string PATH_DELIMETER;
        const static string HTTP_ARG;
        // Project ==============================================================
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
        // Asset ================================================================
        const static string UUID;
        const static string NAME;
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
        const static string ASSET_FORMAT_AUDIO_OGG;
        const static string ASSET_FORMAT_AUDIO_WAV;
        const static string ASSET_FORMAT_ANIMATION_DREAM;
        const static string ASSET_FORMAT_FONT_TTF;
        const static string ASSET_FORMAT_SHADER_GLSL;
        const static string ASSET_FORMAT_LIGHT_POINT;
        const static string ASSET_FORMAT_SPRITE_IMAGE;
        const static string ASSET_DEFINITION_DEFAULT_NAME;

        const static string ASSET_TYPE_ANIMATION_READABLE;
        const static string ASSET_TYPE_AUDIO_READABLE;
        const static string ASSET_TYPE_LIGHT_READABLE;
        const static string ASSET_TYPE_FONT_READABLE;
        const static string ASSET_TYPE_MODEL_READABLE;
        const static string ASSET_TYPE_PHYSICS_OBJECT_READABLE;
        const static string ASSET_TYPE_SCRIPT_READABLE;
        const static string ASSET_TYPE_SHADER_READABLE;
        const static string ASSET_TYPE_SPRITE_READABLE;
        const static string ASSET_FORMAT_READABLE;
        const static string ASSET_FORMAT_SCRIPT_LUA_READABLE;
        const static string ASSET_FORMAT_MODEL_ASSIMP_READABLE;
        const static string ASSET_FORMAT_AUDIO_OGG_READABLE;
        const static string ASSET_FORMAT_AUDIO_WAV_READABLE;
        const static string ASSET_FORMAT_ANIMATION_DREAM_READABLE;
        const static string ASSET_FORMAT_FONT_TTF_READABLE;
        const static string ASSET_FORMAT_SHADER_GLSL_READABLE;
        const static string ASSET_FORMAT_LIGHT_POINT_READABLE;
        const static string ASSET_FORMAT_SPRITE_IMAGE_READABLE;

        const static string getAssetTypeReadableName(string);
        const static string getAssetFormatReadableName(string);

        // Scene ================================================================
        const static string SCENE_NOTES;
        const static string SCENE_SCENE_OBJECTS;
        const static string SCENE_CAMERA;
        const static float  SCENE_CAMERA_DEFAULT_MOVEMENT_SPEED;
        const static string SCENE_TRANSLATION;
        const static string SCENE_ROTATION;
        const static string SCENE_MOVEMENT_SPEED;
        const static string SCENE_CLEAR_COLOUR;
        const static string SCENE_AMBIENT_LIGHT_COLOUR;
        const static string SCENE_GRAVITY;
        const static string SCENE_PHYSICS_DEBUG;
        const static string SCENE_DEFAULT_NAME;
        // SceneObject ==========================================================
        const static string SCENE_OBJECT_ROOT_NAME;
        const static string SCENE_OBJECT_CHILDREN;
        const static string SCENE_OBJECT_HAS_FOCUS;
        const static string SCENE_OBJECT_TRANSLATION;
        const static string SCENE_OBJECT_ROTATION;
        const static string SCENE_OBJECT_SCALE;
        const static string SCENE_OBJECT_TRANSFORM_TYPE;
        const static string SCENE_OBJECT_TRANSFORM_TYPE_OFFSET;
        const static string SCENE_OBJECT_TRANSFORM_TYPE_ABSOLUTE;
        const static string SCENE_OBJECT_ASSET_INSTANCES;
        // Transform ============================================================
        const static string TRANSFORM_TYPE_ABSOLUTE;
        const static string TRANSFORM_TYPE_OFFSET;
        // Font =================================================================
        const static string FONT_SIZE;
        const static string FONT_COLOUR;
        // Generic ==============================================================
        const static string X;
        const static string Y;
        const static string Z;
        const static unsigned int X_INDEX;
        const static unsigned int Y_INDEX;
        const static unsigned int Z_INDEX;
        const static string RED;
        const static string GREEN;
        const static string BLUE;
        const static string ALPHA;
        const static unsigned int RED_INDEX;
        const static unsigned int GREEN_INDEX;
        const static unsigned int BLUE_INDEX;
        const static unsigned int ALPHA_INDEX;
        const static unsigned int XYZ_VECTOR_SIZE;
        const static unsigned int RGB_VECTOR_SIZE;
        const static unsigned int RGBA_VECTOR_SIZE;

        // Animation =============================================================
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
        // Sprite ===============================================================
        const static string SPRITE_TILE_SIZE;
        const static string SPRITE_TILE_WIDTH;
        const static string SPRITE_TILE_HEIGHT;
        // Physics ==============================================================
        const static string ASSET_ATTR_MASS;
        const static string ASSET_ATTR_MARGIN;
        const static string ASSET_ATTR_RADIUS;
        const static string ASSET_ATTR_NORMAL;
        const static string ASSET_ATTR_CONSTANT;
        const static string ASSET_ATTR_SIZE;
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

        const static string COLLISION_SHAPE_SPHERE_READABLE;
        const static string COLLISION_SHAPE_BOX_READABLE;
        const static string COLLISION_SHAPE_CYLINDER_READABLE;
        const static string COLLISION_SHAPE_CAPSULE_READABLE;
        const static string COLLISION_SHAPE_CONE_READABLE;
        const static string COLLISION_SHAPE_MULTI_SPHERE_READABLE;
        const static string COLLISION_SHAPE_CONVEX_HULL_READABLE;
        const static string COLLISION_SHAPE_CONVEX_TRIANGLE_MESH_READABLE;
        const static string COLLISION_SHAPE_BVH_TRIANGLE_MESH_READABLE;
        const static string COLLISION_SHAPE_HEIGHTFIELD_TERRAIN_READABLE;
        const static string COLLISION_SHAPE_STATIC_PLANE_READABLE;
        const static string COLLISION_SHAPE_COMPOUND_READABLE;
        // Event ================================================================
        const static string EVENT_SENDER;
        const static string EVENT_TYPE;
        const static string EVENT_TYPE_COLLISION;
        // Light ================================================================
        const static string ASSET_ATTR_LIGHT_COLOR;
        // Shader ===============================================================
        const static string SHADER_FRAGMENT;
        const static string SHADER_VERTEX;
        // Lua ==================================================================
        const static string LUA_INIT_FUNCTION;
        const static string LUA_UPDATE_FUNCTION;
        const static string LUA_INPUT_FUNCTION;
        const static string LUA_EVENT_FUNCTION;
        // Audio
        const static size_t AUDIO_BUFFER_SIZE;
        // Window
        const static int DEFAULT_SCREEN_WIDTH;
        const static int DEFAULT_SCREEN_HEIGHT;
        // Camera ===============================================================
        const static unsigned int CAMERA_MOVEMENT_FORWARD;
        const static unsigned int CAMERA_MOVEMENT_BACKWARD;
        const static unsigned int CAMERA_MOVEMENT_LEFT;
        const static unsigned int CAMERA_MOVEMENT_RIGHT;
        const static float CAMERA_PITCH_MAX;
        const static float CAMERA_ZOOM_MIN;
        const static float CAMERA_ZOOM_MAX;
        const static float CAMERA_YAW;
        const static float CAMERA_YAW_OFFSET;
        const static float CAMERA_PITCH;
        const static float CAMERA_SPEED;
        const static float CAMERA_SENSITIVTY;
        const static float CAMERA_ZOOM;

        // Data Maps ============================================================
        static map<AssetType,string> DREAM_ASSET_TYPES_MAP;
        static map<AssetType,vector<string>> DREAM_ASSET_FORMATS_MAP;
        static string assetTypeToString(AssetType type);
        static AssetType assetTypeFromString(string type);
    };
}

