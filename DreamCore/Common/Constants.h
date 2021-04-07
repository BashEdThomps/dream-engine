/*
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
#include <map>
#include <vector>

#include "AssetType.h"

using std::map;
using std::string;
using std::vector;

namespace octronic::dream
{
  /**
     * @brief AssetType enum describing the types of Asset available in Dream.
     */


  /**
     * @brief Constants that are used throughout Dream.
     */
  class Constants
  {
  public:

    // Misc ====================================================================
    const static string DIRECTORY_PATH_SEP;
    const static string PATH_DELIMETER;
    // Project =================================================================
    const static string PROJECT_DEFAULT_NAME;
    const static int    PROJECT_UUID_LENGTH;
    const static string PROJECT_FILE_EXTENSION;
    const static string PROJECT_PATH_SEP;
    const static string PROJECT_UUID;
    const static string PROJECT_NAME;
    const static string PROJECT_TEMPLATE_ENTITIES_ARRAY;
    const static string PROJECT_SCENE_ARRAY;
    const static string PROJECT_ANIMATION_ASSET_ARRAY;
    const static string PROJECT_AUDIO_ASSET_ARRAY;
    const static string PROJECT_FONT_ASSET_ARRAY;
    const static string PROJECT_MATERIAL_ASSET_ARRAY;
    const static string PROJECT_MODEL_ASSET_ARRAY;
    const static string PROJECT_PATH_ASSET_ARRAY;
    const static string PROJECT_PHYSICS_ASSET_ARRAY;
    const static string PROJECT_SCRIPT_ASSET_ARRAY;
    const static string PROJECT_SHADER_ASSET_ARRAY;
    const static string PROJECT_TEXTURE_ASSET_ARRAY;
    const static string PROJECT_STARTUP_SCENE;
    // Asset ===================================================================
    const static string UUID;
    const static string NAME;
    const static string ASSET_ATTR_GROUP;
    const static string ASSET_ATTR_GROUP_DEFAULT;
    // Types ===================================================================
    const static string ASSET_TYPE;
    const static string ASSET_TYPE_ANIMATION;
    const static string ASSET_TYPE_AUDIO;
    const static string ASSET_TYPE_FONT;
    const static string ASSET_TYPE_MODEL;
    const static string ASSET_TYPE_PATH;
    const static string ASSET_TYPE_PHYSICS;
    const static string ASSET_TYPE_SCRIPT;
    const static string ASSET_TYPE_SHADER;
    const static string ASSET_TYPE_TEXTURE;
    const static string ASSET_TYPE_MATERIAL;
    // Formats =================================================================
    const static string ASSET_FORMAT;
    const static string ASSET_FORMAT_DREAM;
    const static string ASSET_FORMAT_SCRIPT_LUA;
    const static string ASSET_FORMAT_MODEL_ASSIMP;
    const static string ASSET_FORMAT_AUDIO_OGG;
    const static string ASSET_FORMAT_AUDIO_WAV;
    const static string ASSET_FORMAT_FONT_TTF;
    const static string ASSET_FORMAT_SHADER_GLSL;
    const static string ASSET_FORMAT_SHADER_GLES;
    const static string ASSET_DEFINITION_DEFAULT_NAME;
    const static string ASSET_FORMAT_READABLE;
    const static string ASSET_FORMAT_SCRIPT_LUA_READABLE;
    const static string ASSET_FORMAT_MODEL_ASSIMP_READABLE;
    const static string ASSET_FORMAT_AUDIO_OGG_READABLE;
    const static string ASSET_FORMAT_AUDIO_WAV_READABLE;
    const static string ASSET_FORMAT_DREAM_READABLE;
    const static string ASSET_FORMAT_FONT_TTF_READABLE;
    const static string ASSET_FORMAT_SHADER_GLSL_READABLE;
    // Readable Names ==========================================================
    const static string ASSET_TYPE_ANIMATION_READABLE;
    const static string ASSET_TYPE_PATH_READABLE;
    const static string ASSET_TYPE_AUDIO_READABLE;
    const static string ASSET_TYPE_FONT_READABLE;
    const static string ASSET_TYPE_MODEL_READABLE;
    const static string ASSET_TYPE_PHYSICS_READABLE;
    const static string ASSET_TYPE_SCRIPT_READABLE;
    const static string ASSET_TYPE_SHADER_READABLE;
    const static string ASSET_TYPE_TEXTURE_READABLE;
    const static string ASSET_TYPE_MATERIAL_READABLE;
    // Scene ===================================================================
    const static string SCENE_ROOT_SCENE_ENTITY;
    const static string SCENE_CLEAR_COLOR;
    const static string SCENE_GRAVITY;
    const static string SCENE_DEFAULT_NAME;
    const static string SCENE_CAMERA;
    const static string SCENE_SHADOW_PASS_SHADER;
    const static string SCENE_FONT_SHADER;
    const static string SCENE_SPRITE_SHADER;
    const static string SCENE_INPUT_SCRIPT;
    const static string SCENE_ENVIRONMENT_TEXTURE;
    const static string SCENE_ENVIRONMENT_SHADER;
    // Camera ==================================================================
    const static string CAMERA_FOV;
    const static string CAMERA_MESH_CULL_DISTANCE;
    const static string CAMERA_MIN_DRAW_DISTANCE;
    const static string CAMERA_MAX_DRAW_DISTANCE;
    const static string CAMERA_FREE_TRANSFORM;
    const static string CAMERA_ENTITY_UUID;
    const static string CAMERA_USE_ENTITY;
    // Entity ==================================================================
    const static string TEMPLATE_ENTITY;
    const static string TEMPLATE_ENTITY_DEFAULT_NAME;
    const static string TEMPLATE_ENTITY_ASSET_INSTANCES;
    // Entity Instance =========================================================
    const static string SCENE_ENTITY_DEFINITION;
    const static string SCENE_ENTITY_DEFINITION_DEFAULT_NAME;
    const static string SCENE_ENTITY_DEFINITION_TEMPLATE_UUID;
    const static string SCENE_ENTITY_DEFINITION_TRANSFORM;
    const static string SCENE_ENTITY_DEFINITION_CHILDREN;
    const static string SCENE_ENTITY_DEFINITION_FONT_COLOR;
    const static string SCENE_ENTITY_DEFINITION_FONT_TEXT;
    const static string SCENE_ENTITY_DEFINITION_FONT_SCALE;
    // Transform ===============================================================
    const static string TRANSFORM;
    const static string TRANSLATION;
    const static string YAW;
    const static string PITCH;
    const static string ROLL;
    const static string SCALE;
    const static string X;
    const static string Y;
    const static string Z;
    const static string W;
    // Font ====================================================================
    const static string ASSET_ATTR_FONT_SHADER_ID;
    const static string ASSET_ATTR_FONT_SIZE;
    // Sprite ==================================================================
    const static string ASSET_ATTR_SPRITE_WIDTH;
    const static string ASSET_ATTR_SPRITE_HEIGHT;
    const static string ASSET_ATTR_SPRITE_TEXTURE;
    // Path ====================================================================
    const static string ASSET_ATTR_ID;
    const static string ASSET_ATTR_VELOCITY;
    const static string ASSET_ATTR_POSITION;
    const static string ASSET_ATTR_KEYFRAMES;
    const static string ASSET_ATTR_START_TIME;
    const static string ASSET_ATTR_WRAP;
    const static string ASSET_ATTR_LOOP;
    const static string ASSET_ATTR_INTERPOLATION;
    const static string ASSET_ATTR_CONTROL_POINTS;
    const static string ASSET_ATTR_INDEX;
    const static int DREAM_PATH_STOPPED;
    const static int DREAM_PATH_PLAYING;
    const static int DREAM_PATH_PAUSED;
    const static int MAX_FRAME_ADVANCE;
    const static string DREAM_PATH_SPLINE_TYPE;
    const static string DREAM_PATH_TYPE_OPEN;
    const static string DREAM_PATH_TYPE_CLAMPED;
    const static string DREAM_PATH_TYPE_BEZIER;
    const static string ASSET_ATTR_STEP_SCALAR;
    // Physics =================================================================
    const static string ASSET_ATTR_MASS;
    const static string ASSET_ATTR_MARGIN;
    const static string ASSET_ATTR_RADIUS;
    const static string ASSET_ATTR_HEIGHT;
    const static string ASSET_ATTR_NORMAL;
    const static string ASSET_ATTR_CONSTANT;
    const static string ASSET_ATTR_SIZE;
    const static string ASSET_ATTR_COMPOUND_CHILDREN;
    const static string ASSET_ATTR_COLLISION_MODEL;
    const static string ASSET_ATTR_KINEMATIC;
    const static string ASSET_ATTR_CONTROLLABLE;
    const static string ASSET_ATTR_RESTITUTION;
    const static string ASSET_ATTR_FRICTION;
    const static string ASSET_ATTR_CCD_SPR;
    const static string ASSET_ATTR_ANGULAR_FENTITY;
    const static string ASSET_ATTR_LINEAR_FENTITY;
    const static string ASSET_ATTR_ANGULAR_VELOCITY;
    const static string ASSET_ATTR_LINEAR_VELOCITY;
    // Physics Shapes ==========================================================
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
    // Event ===================================================================
    const static string EVENT_SENDER;
    const static string EVENT_TYPE;
    const static string EVENT_TYPE_COLLISION;
    // Material ================================================================
    const static string ASSET_ATTR_MATERIAL_SHADER;
    const static string ASSET_ATTR_MATERIAL_ALBEDO;
    const static string ASSET_ATTR_MATERIAL_NORMAL;
    const static string ASSET_ATTR_MATERIAL_METALLIC;
    const static string ASSET_ATTR_MATERIAL_ROUGHNESS;
    const static string ASSET_ATTR_MATERIAL_AO;
    // Model ===================================================================
    const static string ASSET_ATTR_MODEL_MATERIAL_LIST;
    const static string ASSET_ATTR_MODEL_MODEL_MATERIAL;
    const static string ASSET_ATTR_MODEL_DREAM_MATERIAL;
    // Shader ==================================================================
    const static string SHADER_FRAGMENT;
    const static string SHADER_VERTEX;
    const static string SHADER_GLSL_FRAGMENT_FILE_NAME;
    const static string SHADER_GLES_FRAGMENT_FILE_NAME;
    const static string SHADER_GLSL_VERTEX_FILE_NAME;
    const static string SHADER_GLES_VERTEX_FILE_NAME;
    // Script ==================================================================
    const static string SCRIPT_INIT_FUNCTION;
    const static string SCRIPT_UPDATE_FUNCTION;
    const static string SCRIPT_INPUT_FUNCTION;
    const static string SCRIPT_EVENT_FUNCTION;
    const static string SCRIPT_DESTROY_FUNCTION;
    // Audio ===================================================================
    const static string ASSET_ATTR_SPECTRUM_ANALYSER;
    const static string ASSET_ATTR_AUDIO_EVENT_MARKERS;
    const static string ASSET_ATTR_AUDIO_EM_INDEX;
    const static string ASSET_ATTR_AUDIO_EM_S_INDEX;
    const static string ASSET_ATTR_AUDIO_EM_NAME;
    const static string ASSET_ATTR_AUDIO_EM_REPEAT;
    const static string ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD;
    // Animation/Keyframe ======================================================
    const static string KEYFRAME_ID;
    const static string KEYFRAME_TIME;
    const static string KEYFRAME_VALUE;
    const static string KEYFRAME_DURATION;
    const static string KEYFRAME_LOOPING;
    const static string KEYFRAME_EASING_TYPE;
    const static string ANIMATION_RELATIVE;
    // Texture =================================================================
    const static string ASSET_ATTR_TEXTURE_EQUI_TO_CUBEMAP_SHADER;
    const static string ASSET_ATTR_TEXTURE_IRRADIANCE_MAP_SHADER;
    const static string ASSET_ATTR_TEXTURE_PREFILTER_SHADER;
    const static string ASSET_ATTR_TEXTURE_BRDF_LUT_SHADER;
    const static string ASSET_ATTR_TEXTURE_IS_ENVIRONMENT;
    const static string ASSET_ATTR_TEXTURE_FLIP_VERTICAL;
    // Data Maps ===============================================================
    static map<AssetType, vector<string>> DREAM_ASSET_FORMATS_MAP;
    static vector<string> DREAM_PATH_SPLINE_TYPES;
    // Funcitons ===============================================================
    static string getAssetFormatStringFromReadableName(const string& format);
    static string getAssetFormatReadableNameFromString(const string&);
  };
}

