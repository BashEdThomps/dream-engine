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

#include "Constants.h"

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif

#ifdef _WIN32
	#ifndef NOMINMAX
		#define NOMINMAX // prevent windows redefining min/max
	#endif

	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

    #include <windows.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif


using namespace std;

namespace Dream
{

#ifdef DREAM_LOG
    bool
    Constants::checkGLError_
    (string file, int line)
    {
        GLenum errorCode = 0;
        bool wasError = false;
        do
        {
            errorCode = glGetError();
            if (errorCode!=0)
            {
                logger->error("OpenGL Error Check {}:{} ", file, line);
                switch (errorCode)
                {
                    case GL_NO_ERROR:
                        logger->error("\tGL_NO_ERROR");
                        break;
                    case GL_INVALID_ENUM:
                        logger->error("\tGL_INVALID_ENUM" );
                        break;
                    case GL_INVALID_VALUE:
                        logger->error("\tGL_INVALID_VALUE");
                        break;
                    case GL_INVALID_OPERATION:
                        logger->error("\tGL_INVALID_OPERATION");
                        break;
#ifndef WIN32
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        logger->error("\tGL_INVALID_FRAMEBUFFER_OPERATION");
                        break;
#endif
                    case GL_OUT_OF_MEMORY:
                        logger->error("\tGL_OUT_OF_MEMORY");
                        break;
                }
                //logger->error("\tName: {}" , glewGetErrorString(errorCode) );
                logger->error("\tCode: {}" , errorCode );
                wasError = true;
            }
        }
        while(errorCode != 0);
        return wasError;
    }
#endif


    // Misc =====================================================================
    const string Constants::DIR_PATH_SEP = "/";
    const string Constants::DIR_PATH_SEP_WINDOWS = "\\";
    const string Constants::PATH_DELIMETER = "::";
    const string Constants::HTTP_ARG = "--http";

    // Project ==================================================================
    const string Constants::PROJECT_DEFAULT_NAME = "Untitled Project";
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
    const string Constants::PROJECT_WINDOW_WIDTH = "width";
    const string Constants::PROJECT_WINDOW_HEIGHT = "height";
    const string Constants::PROJECT_CAPTURE_JOYSTICK = "capture_joystick";
    const string Constants::PROJECT_CAPTURE_KEYBOARD = "capture_keyboard";
    const string Constants::PROJECT_CAPTURE_MOUSE = "capture_mouse";
    const int    Constants::PROJECT_DEFAULT_WINDOW_WIDTH = 1280;
    const int    Constants::PROJECT_DEFAULT_WINDOW_HEIGHT = 720;

    // Asset ====================================================================
    const string Constants::UUID = "uuid";
    const string Constants::NAME = "name";

    // Types
    const string Constants::ASSET_TYPE = "type";
    const string Constants::ASSET_TYPE_ANIMATION = "animation";
    const string Constants::ASSET_TYPE_PATH = "path";
    const string Constants::ASSET_TYPE_AUDIO = "audio";
    const string Constants::ASSET_TYPE_LIGHT = "light";
    const string Constants::ASSET_TYPE_FONT = "font";
    const string Constants::ASSET_TYPE_MODEL = "model";
    const string Constants::ASSET_TYPE_PHYSICS_OBJECT = "physicsObject";
    const string Constants::ASSET_TYPE_SCRIPT = "script";
    const string Constants::ASSET_TYPE_SCROLLER = "scroller";
    const string Constants::ASSET_TYPE_SHADER = "shader";

    const string Constants::ASSET_TYPE_TEXTURE = "texture";
    const string Constants::ASSET_TYPE_MATERIAL = "material";
    const string Constants::ASSET_TYPE_PARTICLE_EMITTER = "particle_emitter";

    // Formats
    const string Constants::ASSET_FORMAT = "format";
    const string Constants::ASSET_FORMAT_SCRIPT_LUA = "lua";
    const string Constants::ASSET_FORMAT_MODEL_ASSIMP = "assimp";
    const string Constants::ASSET_FORMAT_MODEL_OBJ = "assimp.obj";
    const string Constants::ASSET_FORMAT_AUDIO_WAV = "wav";
    const string Constants::ASSET_FORMAT_AUDIO_OGG = "ogg";
    const string Constants::ASSET_FORMAT_DREAM = "dream";
    const string Constants::ASSET_FORMAT_FONT_TTF = "ttf";
    const string Constants::ASSET_FORMAT_SHADER_GLSL = "glsl";
    const string Constants::ASSET_FORMAT_LIGHT_POINT = "point";
    const string Constants::ASSET_FORMAT_LIGHT_DIRECTIONAL = "directional";
    const string Constants::ASSET_FORMAT_LIGHT_SPOTLIGHT = "spotlight";
    const string Constants::ASSET_ATTR_LIGHT_DIRECTION = "direction";
    const string Constants::ASSET_ATTR_LIGHT_CONSTANT = "constant";
    const string Constants::ASSET_ATTR_LIGHT_LINEAR = "linear";
    const string Constants::ASSET_ATTR_LIGHT_QUADRATIC = "quadratic";
    const string Constants::ASSET_ATTR_LIGHT_CUTOFF = "cutoff";
    const string Constants::ASSET_ATTR_LIGHT_OUTER_CUTOFF = "outer_cutoff";

    const string Constants::ASSET_TYPE_ANIMATION_READABLE = "Animation";
    const string Constants::ASSET_TYPE_PATH_READABLE = "Path";
    const string Constants::ASSET_TYPE_AUDIO_READABLE = "Audio";
    const string Constants::ASSET_TYPE_LIGHT_READABLE = "Light";
    const string Constants::ASSET_TYPE_FONT_READABLE = "Font";
    const string Constants::ASSET_TYPE_MODEL_READABLE = "Model";
    const string Constants::ASSET_TYPE_PHYSICS_OBJECT_READABLE = "Physics Object";
    const string Constants::ASSET_TYPE_SCRIPT_READABLE = "Script";
    const string Constants::ASSET_TYPE_SCROLLER_READABLE = "Scroller";
    const string Constants::ASSET_TYPE_SHADER_READABLE = "Shader";
    const string Constants::ASSET_DEFINITION_DEFAULT_NAME = "New Asset Definition";
    const string Constants::ASSET_TYPE_TEXTURE_READABLE = "Texture";
    const string Constants::ASSET_TYPE_MATERIAL_READABLE = "Material";
    const string Constants::ASSET_TYPE_PARTICLE_EMITTER_READABLE = "Particle Emitter";
    const string Constants::ASSET_ATTR_GROUP = "group";
    const string Constants::ASSET_ATTR_GROUP_DEFAULT = "None";

    const string Constants::ASSET_FORMAT_SCRIPT_LUA_READABLE = "Lua";
    const string Constants::ASSET_FORMAT_MODEL_ASSIMP_READABLE = "Assimp Model";
    const string Constants::ASSET_FORMAT_MODEL_OBJ_READABLE = "Wavefront OBJ";
    const string Constants::ASSET_FORMAT_AUDIO_OGG_READABLE = "Ogg";
    const string Constants::ASSET_FORMAT_AUDIO_WAV_READABLE = "Wav";
    const string Constants::ASSET_FORMAT_DREAM_READABLE = "Dream";
    const string Constants::ASSET_FORMAT_FONT_TTF_READABLE = "TTF Font";
    const string Constants::ASSET_FORMAT_SHADER_GLSL_READABLE = "GLSL";
    const string Constants::ASSET_FORMAT_LIGHT_POINT_READABLE = "Point Light";
    const string Constants::ASSET_FORMAT_LIGHT_DIRECTIONAL_READABLE = "Directional Light";
    const string Constants::ASSET_FORMAT_LIGHT_SPOTLIGHT_READABLE = "Spotlight";

    // Scene ====================================================================
    const string Constants::SCENE_NOTES = "notes";
    const string Constants::SCENE_ROOT_SCENE_OBJECT = "root";
    const float  Constants::SCENE_CAMERA_DEFAULT_MOVEMENT_SPEED = 10.0f;
    const string Constants::SCENE_CAMERA_MOVEMENT_SPEED = "cameraMovementSpeed";
    const string Constants::SCENE_CAMERA_TRANSFORM = "cameraTransform";
    const string Constants::SCENE_CLEAR_COLOUR = "clearColour";
    const string Constants::SCENE_AMBIENT_LIGHT_COLOUR = "ambientLight";
    const string Constants::SCENE_GRAVITY = "gravity";
    const string Constants::SCENE_PHYSICS_DEBUG = "physicsDebug";
    const string Constants::SCENE_DEFAULT_NAME = "Untitled Scene";
    const string Constants::SCENE_MESH_CULL_DISTANCE = "mesh_cull_distance";
    const string Constants::SCENE_MIN_DRAW_DISTANCE = "min_draw";
    const string Constants::SCENE_MAX_DRAW_DISTANCE = "max_draw";
    const string Constants::SCENE_CAMERA_TRANSLATION= "camera_translation";
    const string Constants::SCENE_CAMERA_LOOK_AT = "camera_look_at";
    const string Constants::SCENE_CAMERA_PITCH = "camera_pitch";
    const string Constants::SCENE_CAMERA_YAW = "camera_yaw";
    const string Constants::SCENE_CAMERA_FOCUSED_ON = "camera_focused_on";
    const string Constants::SCENE_LIGHTING_PASS_SHADER = "lighting_shader";
    const string Constants::SCENE_SHADOW_PASS_SHADER = "shadow_pass_shader";
    const string Constants::SCENE_INPUT_SCRIPT = "input_script";
    const string Constants::SCENE_NANOVG_SCRIPT = "nanovg_script";

    // SceneObject ==============================================================
    const string Constants::SCENE_OBJECT = "scene_object";
    const string Constants::SCENE_OBJECT_ROOT_NAME = "Root";
    const string Constants::SCENE_OBJECT_DEFAULT_NAME = "New Scene Object";
    const string Constants::SCENE_OBJECT_CHILDREN = "children";
    const string Constants::SCENE_OBJECT_HAS_CAMERA_FOCUS = "camera_focus";
    const string Constants::SCENE_OBJECT_ASSET_INSTANCES = "assetInstances";
    const string Constants::SCENE_OBJECT_ALWAYS_DRAW = "always_draw";
    const string Constants::SCENE_OBJECT_TEMPLATE = "template";
    const string Constants::SCENE_OBJECT_HIDDEN = "hidden";
    const string Constants::SCENE_OBJECT_DEFERRED = "deferred";
    const string Constants::SCENE_OBJECT_DIE_AFTER = "die_after";

    // Transform ================================================================
    const string Constants::TRANSFORM = "transform";
    const string Constants::TRANSFORM_MATRIX = "matrix";
    // Generic ==================================================================
    const string Constants::W = "w";
    const string Constants::X = "x";
    const string Constants::Y = "y";
    const string Constants::Z = "z";
    const unsigned int Constants::X_INDEX = 0;
    const unsigned int Constants::Y_INDEX = 1;
    const unsigned int Constants::Z_INDEX = 2;
    const unsigned int Constants::W_INDEX = 3;
    const string Constants::RED   = "r";
    const string Constants::GREEN = "g";
    const string Constants::BLUE  = "b";
    const string Constants::ALPHA = "a";
    const unsigned int Constants::RED_INDEX   = 0;
    const unsigned int Constants::GREEN_INDEX = 1;
    const unsigned int Constants::BLUE_INDEX  = 2;
    const unsigned int Constants::ALPHA_INDEX = 3;
    const unsigned int Constants::XYZ_VECTOR_SIZE = 3;
    const unsigned int Constants::RGB_VECTOR_SIZE = 3;
    const unsigned int Constants::RGBA_VECTOR_SIZE = 4;

    // Path ================================================================
    const string Constants::ASSET_ATTR_KEYFRAMES     = "keyframes";
    const string Constants::ASSET_ATTR_START_TIME    = "startTime";
    const string Constants::ASSET_ATTR_WRAP          = "wrap";
    const string Constants::ASSET_ATTR_LOOP          = "loop";
    const string Constants::ASSET_ATTR_INTERPOLATION = "interpolation";
    const string Constants::ASSET_ATTR_CONTROL_POINTS = "ctrl_points";
    const string Constants::ASSET_ATTR_INDEX = "index";
    const string Constants::ASSET_ATTR_STEP_SCALAR = "scalar";
    const int Constants::DREAM_PATH_STOPPED = 0;
    const int Constants::DREAM_PATH_PLAYING = 1;
    const int Constants::DREAM_PATH_PAUSED  = 2;
    const int Constants::MAX_FRAME_ADVANCE       = 10;
    const string Constants::DREAM_PATH_SPLINE_TYPE  = "spline_type";
    const string Constants::DREAM_PATH_TYPE_OPEN = "open";
    const string Constants::DREAM_PATH_TYPE_CLAMPED = "clamped";
    const string Constants::DREAM_PATH_TYPE_BEZIER = "bezier";

    // Physics ==================================================================
    const string Constants::ASSET_ATTR_MASS   = "mass";
    const string Constants::ASSET_ATTR_MARGIN = "margin";
    const string Constants::ASSET_ATTR_RADIUS = "radius";
    const string Constants::ASSET_ATTR_HEIGHT = "height";
    const string Constants::ASSET_ATTR_NORMAL = "normal";
    const string Constants::ASSET_ATTR_CONSTANT = "constant";
    const string Constants::ASSET_ATTR_SIZE = "size";
    const string Constants::ASSET_ATTR_COMPOUND_CHILDREN = "compoundChildren";
    const string Constants::ASSET_ATTR_COLLISION_MODEL = "collisionModel";
    const string Constants::ASSET_ATTR_KINEMATIC = "kinematic";
    const string Constants::ASSET_ATTR_CONTROLLABLE = "controllable";
    const string Constants::ASSET_ATTR_CCD_SPR = "ccd_spr";
    const string Constants::ASSET_ATTR_RESTITUTION = "restitution";
    const string Constants::ASSET_ATTR_FRICTION = "friction";

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

    const string Constants::COLLISION_SHAPE_SPHERE_READABLE = "Sphere";
    const string Constants::COLLISION_SHAPE_BOX_READABLE = "Box";
    const string Constants::COLLISION_SHAPE_CYLINDER_READABLE = "Cylinder";
    const string Constants::COLLISION_SHAPE_CAPSULE_READABLE = "Capsule";
    const string Constants::COLLISION_SHAPE_CONE_READABLE = "Cone";
    const string Constants::COLLISION_SHAPE_MULTI_SPHERE_READABLE = "Multi-Sphere";
    const string Constants::COLLISION_SHAPE_CONVEX_HULL_READABLE = "Convex Hull";
    const string Constants::COLLISION_SHAPE_CONVEX_TRIANGLE_MESH_READABLE = "Convex Triangle Mesh";
    const string Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH_READABLE = "BVH Triangle Mesh";
    const string Constants::COLLISION_SHAPE_HEIGHTFIELD_TERRAIN_READABLE = "Heightfield Terrain";
    const string Constants::COLLISION_SHAPE_STATIC_PLANE_READABLE = "Static Plane";
    const string Constants::COLLISION_SHAPE_COMPOUND_READABLE = "Compound Shape";

    // Event ====================================================================
    const string Constants::EVENT_SENDER = "sender";
    const string Constants::EVENT_TYPE = "type";
    const string Constants::EVENT_TYPE_COLLISION = "collision";

    // Light ====================================================================
    const string Constants::ASSET_ATTR_LIGHT_COLOR   = "colour";
    const string Constants::ASSET_ATTR_LIGHT_DIFFUSE = "diffuse";
    const string  Constants::ASSET_ATTR_LIGHT_AMBIENT = "ambient";
    const string Constants::ASSET_ATTR_LIGHT_SPECULAR = "specular";
    // Material
    const string Constants::ASSET_ATTR_MATERIAL_SHADER = "shader";
    const string Constants::ASSET_ATTR_MATERIAL_DIFFUSE_TEXTURE = "diffuse_texture";
    const string Constants::ASSET_ATTR_MATERIAL_SPECULAR_TEXTURE = "specular_texture";
    const string Constants::ASSET_ATTR_MATERIAL_NORMAL_TEXTURE = "normal_texture";
    const string Constants::ASSET_ATTR_MATERIAL_DISPLACEMENT_TEXTURE = "displacement_texture";
    const string Constants::ASSET_ATTR_MATERIAL_DIFFUSE_COLOUR = "diffuse_colour";
    const string Constants::ASSET_ATTR_MATERIAL_SPECULAR_COLOUR = "specular_colour";
    const string Constants::ASSET_ATTR_MATERIAL_AMBIENT_COLOUR = "ambient_colour";
    const string Constants::ASSET_ATTR_MATERIAL_REFLECTIVE_COLOUR = "reflective_colour";
    const string Constants::ASSET_ATTR_MATERIAL_EMISSIVE_COLOUR = "emissive_colour";
    const string Constants::ASSET_ATTR_MATERIAL_OPACITY = "opacity";
    const string Constants::ASSET_ATTR_MATERIAL_BUMP_SCALING = "bump_scaling";
    const string Constants::ASSET_ATTR_MATERIAL_HARDNESS = "hardness";
    const string Constants::ASSET_ATTR_MATERIAL_REFLECTIVITY = "reflectivity";
    const string Constants::ASSET_ATTR_MATERIAL_SHININESS_STRENGTH = "shininess_strength";
    const string Constants::ASSET_ATTR_MATERIAL_REFRACTION_INDEX = "refraction_index";
    const string Constants::ASSET_ATTR_MATERIAL_IGNORE = "ignore";

    // Model ================================================================
    const string Constants::ASSET_ATTR_MODEL_MATERIAL_LIST = "material_list";
    const string Constants::ASSET_ATTR_MODEL_MODEL_MATERIAL = "model_material";
    const string Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL = "dream_material";

    // Lua ======================================================================
    const string Constants::LUA_INIT_FUNCTION   = "onInit";
    const string Constants::LUA_UPDATE_FUNCTION = "onUpdate";
    const string Constants::LUA_NANOVG_FUNCTION = "onNanoVG";
    const string Constants::LUA_INPUT_FUNCTION  = "onInput";
    const string Constants::LUA_EVENT_FUNCTION  = "onEvent";

    // Scroller ================================================================
    const string Constants::SCROLLER_VELOCITY = "velocity";
    const string Constants::SCROLLER_LOOP = "loop";
    const string Constants::SCROLLER_RANGE_BEGIN = "range_begin";
    const string Constants::SCROLLER_RANGE_END = "range_end";
    const string Constants::SCROLLER_ITEMS_ARRAY = "items";
    const string Constants::SCROLLER_ITEM_INDEX = "index";
    const string Constants::SCROLLER_ITEM_ORIGIN = "origin";

    // Shader ===================================================================
    const string Constants::SHADER_FRAGMENT = ".frag";
    const string Constants::SHADER_VERTEX   = ".vert";

    const string Constants::SHADER_FRAGMENT_FILE_NAME
        = Constants::ASSET_FORMAT_SHADER_GLSL +
          Constants::SHADER_FRAGMENT ;

    const string Constants::SHADER_VERTEX_FILE_NAME
        = Constants::ASSET_FORMAT_SHADER_GLSL +
          Constants::SHADER_VERTEX;

    // Audio ====================================================================
    const string Constants::ASSET_ATTR_SPECTRUM_ANALYSER = "spectrum_analyser";
    const string Constants::ASSET_ATTR_AUDIO_EVENT_MARKERS = "event_markers";
    const string Constants::ASSET_ATTR_AUDIO_EM_INDEX = "index";
    const string Constants::ASSET_ATTR_AUDIO_EM_S_INDEX = "sample_index";
    const string Constants::ASSET_ATTR_AUDIO_EM_NAME = "name";
    const string Constants::ASSET_ATTR_AUDIO_EM_REPEAT = "repeat";
    const string Constants::ASSET_ATTR_AUDIO_EM_REPEAT_PERIOD = "repeat_period";

    // Particle Emitter ========================================================
    const string Constants::PARTICLE_EMITTER_TEXTURE = "texture";
    const string Constants::PARTICLE_EMITTER_AREA = "area";
    const string Constants::PARTICLE_EMITTER_SIZE = "size";
    const string Constants::PARTICLE_EMITTER_VELOCITY = "velocity";
    const string Constants::PARTICLE_EMITTER_GRAVITY = "gravity";
    const string Constants::PARTICLE_EMITTER_PER_SECOND = "per_second";
    const string Constants::PARTICLE_EMITTER_LIFETIME = "lifetime";

    // Animation / Keyframe ====================================================
    const string Constants::KEYFRAME_ID = "kf_id";
    const string Constants::KEYFRAME_TRANSLATION = "translation";
    const string Constants::KEYFRAME_ROTATION = "rotation";
    const string Constants::KEYFRAME_SCALE = "scale";
    const string Constants::KEYFRAME_TIME = "time";
    const string Constants::KEYFRAME_DURATION = "duration";
    const string Constants::KEYFRAME_LOOPING = "looping";
    const string Constants::KEYFRAME_EASING_TYPE = "easing_type";
    const string Constants::ANIMATION_RELATIVE = "relative";

    // Window ===================================================================
    const int Constants::DEFAULT_SCREEN_WIDTH  = 1280;
    const int Constants::DEFAULT_SCREEN_HEIGHT =  720;

    // Camera ===================================================================
    const unsigned int Constants::CAMERA_MOVEMENT_FORWARD  = 0;
    const unsigned int Constants::CAMERA_MOVEMENT_BACKWARD = 1;
    const unsigned int Constants::CAMERA_MOVEMENT_LEFT     = 2;
    const unsigned int Constants::CAMERA_MOVEMENT_RIGHT    = 3;
    const unsigned int Constants::CAMERA_MOVEMENT_UP       = 4;
    const unsigned int Constants::CAMERA_MOVEMENT_DOWN     = 5;

    const float Constants::CAMERA_PITCH_MAX = 1.565f;
    const float Constants::CAMERA_ZOOM_MIN = 1.0f;
    const float Constants::CAMERA_ZOOM_MAX = 45.0f;
    const float Constants::CAMERA_YAW = 00.0f;
    const float Constants::CAMERA_YAW_OFFSET = 180.0f;
    const float Constants::CAMERA_PITCH = 0.0f;
    const float Constants::CAMERA_SPEED = 10.0f;
    const float Constants::CAMERA_SENSITIVTY = 0.001f;
    const float Constants::CAMERA_ZOOM = 45.0f;

    // Data =====================================================================

    map<AssetType,string> Constants::DREAM_ASSET_TYPES_MAP =
    {
        {AssetType::ANIMATION,        ASSET_TYPE_ANIMATION},
        {AssetType::AUDIO,            ASSET_TYPE_AUDIO},
        {AssetType::FONT,             ASSET_TYPE_FONT},
        {AssetType::LIGHT,            ASSET_TYPE_LIGHT},
        {AssetType::MATERIAL,         ASSET_TYPE_MATERIAL},
        {AssetType::MODEL,            ASSET_TYPE_MODEL},
        {AssetType::PARTICLE_EMITTER, ASSET_TYPE_PARTICLE_EMITTER},
        {AssetType::PATH,             ASSET_TYPE_PATH},
        {AssetType::PHYSICS_OBJECT,   ASSET_TYPE_PHYSICS_OBJECT},
        {AssetType::SCRIPT,           ASSET_TYPE_SCRIPT},
        {AssetType::SCROLLER,         ASSET_TYPE_SCROLLER},
        {AssetType::SHADER,           ASSET_TYPE_SHADER},
        {AssetType::TEXTURE,          ASSET_TYPE_TEXTURE}
    };

    vector<string> Constants::DREAM_ASSET_TYPES_READABLE_VECTOR =
    {
        ASSET_TYPE_ANIMATION_READABLE,
        ASSET_TYPE_AUDIO_READABLE,
        ASSET_TYPE_FONT_READABLE,
        ASSET_TYPE_LIGHT_READABLE,
        ASSET_TYPE_MATERIAL_READABLE,
        ASSET_TYPE_MODEL_READABLE,
        ASSET_TYPE_PARTICLE_EMITTER_READABLE,
        ASSET_TYPE_PATH_READABLE,
        ASSET_TYPE_PHYSICS_OBJECT_READABLE,
        ASSET_TYPE_SCRIPT_READABLE,
        ASSET_TYPE_SCROLLER_READABLE,
        ASSET_TYPE_SHADER_READABLE,
        ASSET_TYPE_TEXTURE_READABLE
    };

    map<AssetType,vector<string>> Constants::DREAM_ASSET_FORMATS_MAP =
    {
        {
            AssetType::ANIMATION,
            {
                ASSET_FORMAT_DREAM
            }
        },
        {
            AssetType::AUDIO,
            {
                ASSET_FORMAT_AUDIO_WAV,
                ASSET_FORMAT_AUDIO_OGG
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
                ASSET_FORMAT_LIGHT_POINT,
                ASSET_FORMAT_LIGHT_DIRECTIONAL,
                ASSET_FORMAT_LIGHT_SPOTLIGHT
            }
        },
        {
            AssetType::MATERIAL,
            {
                ASSET_FORMAT_DREAM
            }
        },
        {
            AssetType::MODEL,
            {
                //ASSET_FORMAT_MODEL_ASSIMP,
                ASSET_FORMAT_MODEL_OBJ
            }
        },
        {
            AssetType::PARTICLE_EMITTER,
            {
                ASSET_FORMAT_DREAM
            }
        },
        {
            AssetType::PATH,
            {
                ASSET_FORMAT_DREAM
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
            AssetType::SCROLLER,
            {
                ASSET_FORMAT_DREAM
            }
        },
        {
            AssetType::SHADER,
            {
                ASSET_FORMAT_SHADER_GLSL
            }
        },
        {
            AssetType::TEXTURE,
            {
                ASSET_FORMAT_DREAM
            }
        }
    };

    string
    Constants::getAssetTypeStringFromTypeEnum
    (AssetType type)
    {
        switch (type)
        {
            case ANIMATION:
                return ASSET_TYPE_ANIMATION;
            case PATH:
                return ASSET_TYPE_PATH;
            case AUDIO:
                return ASSET_TYPE_AUDIO;
            case FONT:
                return ASSET_TYPE_FONT;
            case LIGHT:
                return ASSET_TYPE_LIGHT;
            case MATERIAL:
                return ASSET_TYPE_MATERIAL;
            case MODEL:
                return ASSET_TYPE_MODEL;
            case PARTICLE_EMITTER:
                return ASSET_TYPE_PARTICLE_EMITTER;
            case PHYSICS_OBJECT:
                return ASSET_TYPE_PHYSICS_OBJECT;
            case SCRIPT:
                return ASSET_TYPE_SCRIPT;
            case SCROLLER:
                return ASSET_TYPE_SCROLLER;
            case SHADER:
                return ASSET_TYPE_SHADER;
            case TEXTURE:
                return ASSET_TYPE_TEXTURE;
            case NONE:
                return "";
        }
        return "";
    }

    AssetType
    Constants::getAssetTypeEnumFromString
    (string type)
    {
        if (type.compare(ASSET_TYPE_ANIMATION) == 0 ||
            type.compare(ASSET_TYPE_ANIMATION_READABLE) == 0)
        {
            return ANIMATION;
        }

        else if (type.compare(ASSET_TYPE_AUDIO) == 0 ||
            type.compare(ASSET_TYPE_AUDIO_READABLE) == 0)
        {
            return AUDIO;
        }

        else if (type.compare(ASSET_TYPE_FONT) == 0 ||
                 type.compare(ASSET_TYPE_FONT_READABLE) == 0)
        {
            return FONT;
        }

        else if (type.compare(ASSET_TYPE_LIGHT) == 0 ||
                 type.compare(ASSET_TYPE_LIGHT_READABLE) == 0)
        {
            return LIGHT;
        }

        else if (type.compare(ASSET_TYPE_MATERIAL) == 0 ||
                 type.compare(ASSET_TYPE_MATERIAL_READABLE) == 0)
        {
            return MATERIAL;
        }

        else if (type.compare(ASSET_TYPE_MODEL) == 0 ||
                 type.compare(ASSET_TYPE_MODEL_READABLE) == 0)
        {
            return MODEL;
        }

        else if (type.compare(ASSET_TYPE_PARTICLE_EMITTER) == 0 ||
                 type.compare(ASSET_TYPE_PARTICLE_EMITTER_READABLE) == 0)
        {
            return PARTICLE_EMITTER;
        }

        else if (type.compare(ASSET_TYPE_PATH) == 0 ||
            type.compare(ASSET_TYPE_PATH_READABLE) == 0)
        {
            return PATH;
        }

        else if (type.compare(ASSET_TYPE_PHYSICS_OBJECT) == 0 ||
                 type.compare(ASSET_TYPE_PHYSICS_OBJECT_READABLE) == 0)
        {
            return PHYSICS_OBJECT;
        }

        else if (type.compare(ASSET_TYPE_SCRIPT) == 0 ||
                 type.compare(ASSET_TYPE_SCRIPT_READABLE) == 0)
        {
            return SCRIPT;
        }

        else if (type.compare(ASSET_TYPE_SCROLLER) == 0 ||
                 type.compare(ASSET_TYPE_SCROLLER_READABLE) == 0)
        {
            return SCROLLER;
        }

        else if (type.compare(ASSET_TYPE_SHADER) == 0 ||
                 type.compare(ASSET_TYPE_SHADER_READABLE) == 0)
        {
            return SHADER;
        }

        else if (type.compare(ASSET_TYPE_TEXTURE) == 0 ||
                 type.compare(ASSET_TYPE_TEXTURE_READABLE) == 0)
        {
            return TEXTURE;
        }
        return NONE;
    }

    string
    Constants::getAssetTypeReadableNameFromString
    (string type)
    {
        if (type.compare(ASSET_TYPE_ANIMATION) == 0)
        {
            return ASSET_TYPE_ANIMATION_READABLE;
        }

        else if (type.compare(ASSET_TYPE_AUDIO) == 0)
        {
            return ASSET_TYPE_AUDIO_READABLE;
        }

        else if (type.compare(ASSET_TYPE_FONT) == 0)
        {
            return ASSET_TYPE_FONT_READABLE;
        }

        else if (type.compare(ASSET_TYPE_LIGHT) == 0)
        {
            return ASSET_TYPE_LIGHT_READABLE;
        }

        else if (type.compare(ASSET_TYPE_MATERIAL) == 0)
        {
            return ASSET_TYPE_MATERIAL_READABLE;
        }

        else if (type.compare(ASSET_TYPE_MODEL) == 0)
        {
            return ASSET_TYPE_MODEL_READABLE;
        }

        else if (type.compare(ASSET_TYPE_PARTICLE_EMITTER) == 0)
        {
            return ASSET_TYPE_PARTICLE_EMITTER_READABLE;
        }

        else if (type.compare(ASSET_TYPE_PATH) == 0)
        {
            return ASSET_TYPE_PATH_READABLE;
        }

        else if (type.compare(ASSET_TYPE_PHYSICS_OBJECT) == 0)
        {
            return ASSET_TYPE_PHYSICS_OBJECT_READABLE;
        }

        else if (type.compare(ASSET_TYPE_SCRIPT) == 0)
        {
            return ASSET_TYPE_SCRIPT_READABLE;
        }

        else if (type.compare(ASSET_TYPE_SCROLLER) == 0)
        {
            return ASSET_TYPE_SCROLLER_READABLE;
        }

        else if (type.compare(ASSET_TYPE_SHADER) == 0)
        {
            return ASSET_TYPE_SHADER_READABLE;
        }

        else if (type.compare(ASSET_TYPE_TEXTURE) == 0)
        {
            return ASSET_TYPE_TEXTURE_READABLE;
        }
        return "";
    }

    string
    Constants::getAssetFormatReadableNameFromString
    (string format)
    {
        // Script
        if (format.compare(ASSET_FORMAT_SCRIPT_LUA) == 0)
        {
           return ASSET_FORMAT_SCRIPT_LUA_READABLE;
        }
        // Model
        else if (format.compare(ASSET_FORMAT_MODEL_ASSIMP) == 0)
        {
           return ASSET_FORMAT_MODEL_ASSIMP_READABLE;
        }
        else if (format.compare(ASSET_FORMAT_MODEL_OBJ) == 0)
        {
           return ASSET_FORMAT_MODEL_OBJ_READABLE;
        }
        // Audio
        else if (format.compare(ASSET_FORMAT_AUDIO_OGG) == 0)
        {
           return ASSET_FORMAT_AUDIO_OGG_READABLE;
        }
        else if (format.compare(ASSET_FORMAT_AUDIO_WAV) == 0)
        {
           return ASSET_FORMAT_AUDIO_WAV_READABLE;
        }
        // Font
        else if (format.compare(ASSET_FORMAT_FONT_TTF) == 0)
        {
           return ASSET_FORMAT_FONT_TTF_READABLE;
        }
        // Shader
        else if (format.compare(ASSET_FORMAT_SHADER_GLSL) == 0)
        {
           return ASSET_FORMAT_SHADER_GLSL_READABLE;
        }
        // Light
        else if (format.compare(ASSET_FORMAT_LIGHT_POINT) == 0)
        {
           return ASSET_FORMAT_LIGHT_POINT_READABLE;
        }
        else if (format.compare(ASSET_FORMAT_LIGHT_DIRECTIONAL) == 0)
        {
           return ASSET_FORMAT_LIGHT_DIRECTIONAL_READABLE;
        }
        else if (format.compare(ASSET_FORMAT_LIGHT_SPOTLIGHT) == 0)
        {
           return ASSET_FORMAT_LIGHT_SPOTLIGHT_READABLE;
        }
        // Dream
        else if (format.compare(ASSET_FORMAT_DREAM) == 0)
        {
            return ASSET_FORMAT_DREAM_READABLE;
        }
        // Physics
        else if (format.compare(COLLISION_SHAPE_SPHERE) == 0)
        {
            return COLLISION_SHAPE_SPHERE_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_BOX) == 0)
        {
            return COLLISION_SHAPE_BOX_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_CYLINDER) == 0)
        {
            return COLLISION_SHAPE_CYLINDER_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_CAPSULE) == 0)
        {
            return COLLISION_SHAPE_CAPSULE_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_CONE) == 0)
        {
            return COLLISION_SHAPE_MULTI_SPHERE_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_CONVEX_HULL) == 0)
        {
            return COLLISION_SHAPE_CONVEX_HULL_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_CONVEX_TRIANGLE_MESH) == 0)
        {
            return COLLISION_SHAPE_CONVEX_TRIANGLE_MESH_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_BVH_TRIANGLE_MESH) == 0)
        {
            return COLLISION_SHAPE_BVH_TRIANGLE_MESH_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_HEIGHTFIELD_TERRAIN) == 0)
        {
            return COLLISION_SHAPE_HEIGHTFIELD_TERRAIN_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_STATIC_PLANE) == 0)
        {
            return COLLISION_SHAPE_STATIC_PLANE_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_COMPOUND) == 0)
        {
            return COLLISION_SHAPE_COMPOUND_READABLE;
        }
        else if (format.compare(COLLISION_SHAPE_MULTI_SPHERE) == 0)
        {
            return COLLISION_SHAPE_MULTI_SPHERE_READABLE;
        }

        return "";
    }

    string
    Constants::getAssetFormatStringFromReadableName
    (string format)
    {
        if (format.compare(ASSET_FORMAT_SCRIPT_LUA_READABLE) == 0)
        {
           return ASSET_FORMAT_SCRIPT_LUA;
        }
        else if (format.compare(ASSET_FORMAT_MODEL_ASSIMP_READABLE) == 0)
        {
           return ASSET_FORMAT_MODEL_ASSIMP;
        }
        else if (format.compare(ASSET_FORMAT_MODEL_OBJ_READABLE) == 0)
        {
           return ASSET_FORMAT_MODEL_OBJ;
        }
        else if (format.compare(ASSET_FORMAT_AUDIO_OGG_READABLE) == 0)
        {
           return ASSET_FORMAT_AUDIO_OGG;
        }
        else if (format.compare(ASSET_FORMAT_AUDIO_WAV_READABLE) == 0)
        {
           return ASSET_FORMAT_AUDIO_WAV;
        }
        else if (format.compare(ASSET_FORMAT_FONT_TTF_READABLE) == 0)
        {
           return ASSET_FORMAT_FONT_TTF;
        }
        else if (format.compare(ASSET_FORMAT_SHADER_GLSL_READABLE) == 0)
        {
           return ASSET_FORMAT_SHADER_GLSL;
        }
        else if (format.compare(ASSET_FORMAT_LIGHT_POINT_READABLE) == 0)
        {
           return ASSET_FORMAT_LIGHT_POINT;
        }
        else if (format.compare(ASSET_FORMAT_LIGHT_DIRECTIONAL_READABLE) == 0)
        {
           return ASSET_FORMAT_LIGHT_DIRECTIONAL;
        }
        else if (format.compare(ASSET_FORMAT_LIGHT_SPOTLIGHT_READABLE) == 0)
        {
           return ASSET_FORMAT_LIGHT_SPOTLIGHT;
        }
        else if (format.compare(ASSET_FORMAT_DREAM_READABLE) == 0)
        {
            return ASSET_FORMAT_DREAM;
        }
        else if (format.compare(COLLISION_SHAPE_SPHERE_READABLE) == 0)
        {
            return COLLISION_SHAPE_SPHERE;
        }
        else if (format.compare(COLLISION_SHAPE_BOX_READABLE) == 0)
        {
            return COLLISION_SHAPE_BOX;
        }
        else if (format.compare(COLLISION_SHAPE_CYLINDER_READABLE) == 0)
        {
            return COLLISION_SHAPE_CYLINDER;
        }
        else if (format.compare(COLLISION_SHAPE_CAPSULE_READABLE) == 0)
        {
            return COLLISION_SHAPE_CAPSULE;
        }
        else if (format.compare(COLLISION_SHAPE_CONE_READABLE) == 0)
        {
            return COLLISION_SHAPE_CONE;
        }
        else if (format.compare(COLLISION_SHAPE_CONVEX_HULL_READABLE) == 0)
        {
            return COLLISION_SHAPE_CONVEX_HULL;
        }
        else if (format.compare(COLLISION_SHAPE_CONVEX_TRIANGLE_MESH_READABLE) == 0)
        {
            return COLLISION_SHAPE_CONVEX_TRIANGLE_MESH;
        }
        else if (format.compare(COLLISION_SHAPE_BVH_TRIANGLE_MESH_READABLE) == 0)
        {
            return COLLISION_SHAPE_BVH_TRIANGLE_MESH;
        }
        else if (format.compare(COLLISION_SHAPE_HEIGHTFIELD_TERRAIN_READABLE) == 0)
        {
            return COLLISION_SHAPE_HEIGHTFIELD_TERRAIN;
        }
        else if (format.compare(COLLISION_SHAPE_STATIC_PLANE_READABLE) == 0)
        {
            return COLLISION_SHAPE_STATIC_PLANE;
        }
        else if (format.compare(COLLISION_SHAPE_COMPOUND_READABLE) == 0)
        {
            return COLLISION_SHAPE_COMPOUND;
        }
        else if (format.compare(COLLISION_SHAPE_MULTI_SPHERE_READABLE) == 0)
        {
            return COLLISION_SHAPE_MULTI_SPHERE;
        }

        return "";
    }

#ifdef DREAM_LOG
    shared_ptr<spdlog::logger> Constants::logger = spdlog::stdout_color_mt("Constants");
#endif

    vector<string> Constants::DREAM_ANIMATION_SPLINE_TYPES
    {
        DREAM_PATH_TYPE_OPEN,
        DREAM_PATH_TYPE_BEZIER,
        DREAM_PATH_TYPE_CLAMPED
    };

}


