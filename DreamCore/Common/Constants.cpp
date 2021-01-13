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

using namespace std;

namespace Dream
{
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
    const string Constants::ASSET_TYPE_OBJECT_EMITTER = "object_emitter";
    const string Constants::ASSET_TYPE_PHYSICS_OBJECT = "physicsObject";
    const string Constants::ASSET_TYPE_SCRIPT = "script";
    const string Constants::ASSET_TYPE_SCROLLER = "scroller";
    const string Constants::ASSET_TYPE_SHADER = "shader";

    const string Constants::ASSET_TYPE_TEXTURE = "texture";
    const string Constants::ASSET_TYPE_MATERIAL = "material";
    const string Constants::ASSET_TYPE_PARTICLE_EMITTER = "particle_emitter";

    // Formats
    const string Constants::ASSET_FORMAT = "format";
    const string Constants::ASSET_FORMAT_SCRIPT_ANGELSCRIPT = "script.as";
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
    const string Constants::ASSET_TYPE_OBJECT_EMITTER_READABLE = "Object Emitter";
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

    const string Constants::ASSET_FORMAT_SCRIPT_ANGELSCRIPT_READABLE = "AngelScript";
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
    const string Constants::SCENE_ROOT_ENTITY = "root";
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

    // Entity ==============================================================
    const string Constants::ENTITY = "actor";
    const string Constants::ENTITY_ROOT_NAME = "Root Entity";
    const string Constants::ENTITY_DEFAULT_NAME = "New Entity";
    const string Constants::ENTITY_CHILDREN = "children";
    const string Constants::ENTITY_HAS_CAMERA_FOCUS = "camera_focus";
    const string Constants::ENTITY_ASSET_INSTANCES = "assetInstances";
    const string Constants::ENTITY_ALWAYS_DRAW = "always_draw";
    const string Constants::ENTITY_TEMPLATE = "template";
    const string Constants::ENTITY_HIDDEN = "hidden";
    const string Constants::ENTITY_DEFERRED = "deferred";
    const string Constants::ENTITY_DIE_AFTER = "die_after";
    const string Constants::ENTITY_PLAYER_OBJECT = "player_object";

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
    const string Constants::ASSET_ATTR_ID            = "id";
    const string Constants::ASSET_ATTR_VELOCITY      = "velocity";
    const string Constants::ASSET_ATTR_POSITION      = "position";
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
    const string Constants::ASSET_ATTR_ANGULAR_FENTITY = "angular_factor";
    const string Constants::ASSET_ATTR_LINEAR_FENTITY = "linear_factor";
    const string Constants::ASSET_ATTR_ANGULAR_VELOCITY = "angular_velocity";
    const string Constants::ASSET_ATTR_LINEAR_VELOCITY = "linear_velocity";


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

    // Object Emitter ==========================================================
    const string Constants::OBJECT_EMITTER_UUID = "so_uuid";
    const string Constants::OBJECT_EMITTER_OBJECT_COUNT = "object_count";
    const string Constants::OBJECT_EMITTER_EMIT_INTERVAL = "emit_interval";
    const string Constants::OBJECT_EMITTER_LOOP_INTERVAL = "loop_interval";
    const string Constants::OBJECT_EMITTER_LOOPS = "loops";
    const string Constants::OBJECT_EMITTER_VELOCITY = "velocity";
    const string Constants::OBJECT_EMITTER_START_THETA = "start_theta";
    const string Constants::OBJECT_EMITTER_END_THETA = "end_theta";
    const string Constants::OBJECT_EMITTER_START_RADIUS = "start_radius";

    // Model ===================================================================
    const string Constants::ASSET_ATTR_MODEL_MATERIAL_LIST = "material_list";
    const string Constants::ASSET_ATTR_MODEL_MODEL_MATERIAL = "model_material";
    const string Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL = "dream_material";

    // Lua =====================================================================
    const string Constants::SCRIPT_INIT_FUNCTION   = "onInit";
    const string Constants::SCRIPT_UPDATE_FUNCTION = "onUpdate";
    const string Constants::SCRIPT_INPUT_FUNCTION  = "onInput";
    const string Constants::SCRIPT_EVENT_FUNCTION  = "onEvent";
    const string Constants::SCRIPT_DESTROY_FUNCTION  = "onDestroy";

    // Scroller ================================================================
    const string Constants::SCROLLER_VELOCITY = "velocity";
    const string Constants::SCROLLER_LOOP = "loop";
    const string Constants::SCROLLER_RANGE_BEGIN = "range_begin";
    const string Constants::SCROLLER_RANGE_END = "range_end";
    const string Constants::SCROLLER_ITEMS_ARRAY = "items";
    const string Constants::SCROLLER_ITEM_INDEX = "index";
    const string Constants::SCROLLER_ITEM_ORIGIN = "origin";

    // Shader ==================================================================
    const string Constants::SHADER_FRAGMENT = ".frag";
    const string Constants::SHADER_VERTEX   = ".vert";

    const string Constants::SHADER_FRAGMENT_FILE_NAME
        = Constants::ASSET_FORMAT_SHADER_GLSL +
          Constants::SHADER_FRAGMENT ;

    const string Constants::SHADER_VERTEX_FILE_NAME
        = Constants::ASSET_FORMAT_SHADER_GLSL +
          Constants::SHADER_VERTEX;

    // Audio ===================================================================
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

    // Window ==================================================================
    const int Constants::DEFAULT_SCREEN_WIDTH  = 1280;
    const int Constants::DEFAULT_SCREEN_HEIGHT =  720;

    // Camera ==================================================================
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

    // Data ====================================================================

    map<AssetType,string> Constants::DREAM_ASSET_TYPES_MAP =
    {
        {AssetType::ANIMATION,        ASSET_TYPE_ANIMATION},
        {AssetType::AUDIO,            ASSET_TYPE_AUDIO},
        {AssetType::FONT,             ASSET_TYPE_FONT},
        {AssetType::LIGHT,            ASSET_TYPE_LIGHT},
        {AssetType::MATERIAL,         ASSET_TYPE_MATERIAL},
        {AssetType::MODEL,            ASSET_TYPE_MODEL},
        {AssetType::OBJECT_EMITTER,   ASSET_TYPE_OBJECT_EMITTER},
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
        ASSET_TYPE_OBJECT_EMITTER_READABLE,
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
                ASSET_FORMAT_MODEL_OBJ
            }
        },
        {
            AssetType::OBJECT_EMITTER,
            {
                ASSET_FORMAT_DREAM
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
                ASSET_FORMAT_SCRIPT_ANGELSCRIPT
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
            case OBJECT_EMITTER:
                return ASSET_TYPE_OBJECT_EMITTER;
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
    (const string& type)
    {
        AssetType retval = AssetType::NONE;
        if (type == ASSET_TYPE_ANIMATION || type == ASSET_TYPE_ANIMATION_READABLE)
        {
            retval =  ANIMATION;
        }

        else if (type == ASSET_TYPE_AUDIO || type == ASSET_TYPE_AUDIO_READABLE)
        {
            retval = AUDIO;
        }

        else if (type == ASSET_TYPE_FONT || type == ASSET_TYPE_FONT_READABLE)
        {
            retval = FONT;
        }

        else if (type == ASSET_TYPE_LIGHT || type == ASSET_TYPE_LIGHT_READABLE)
        {
            retval = LIGHT;
        }

        else if (type == ASSET_TYPE_MATERIAL || type == ASSET_TYPE_MATERIAL_READABLE)
        {
            retval = MATERIAL;
        }

        else if (type == ASSET_TYPE_MODEL || type == ASSET_TYPE_MODEL_READABLE)
        {
            retval = MODEL;
        }

        else if (type == ASSET_TYPE_OBJECT_EMITTER || type == ASSET_TYPE_OBJECT_EMITTER_READABLE)
        {
            retval = OBJECT_EMITTER;
        }

        else if (type == ASSET_TYPE_PARTICLE_EMITTER || type == ASSET_TYPE_PARTICLE_EMITTER_READABLE)
        {
            retval = PARTICLE_EMITTER;
        }

        else if (type == ASSET_TYPE_PATH || type == ASSET_TYPE_PATH_READABLE)
        {
            retval = PATH;
        }

        else if (type == ASSET_TYPE_PHYSICS_OBJECT || type == ASSET_TYPE_PHYSICS_OBJECT_READABLE)
        {
            retval = PHYSICS_OBJECT;
        }

        else if (type == ASSET_TYPE_SCRIPT || type == ASSET_TYPE_SCRIPT_READABLE)
        {
            retval = SCRIPT;
        }

        else if (type == ASSET_TYPE_SCROLLER || type == ASSET_TYPE_SCROLLER_READABLE)
        {
            retval = SCROLLER;
        }

        else if (type == ASSET_TYPE_SHADER || type == ASSET_TYPE_SHADER_READABLE)
        {
            retval = SHADER;
        }

        else if (type == ASSET_TYPE_TEXTURE || type == ASSET_TYPE_TEXTURE_READABLE)
        {
            retval =  TEXTURE;
        }
        return retval;
    }

    string
    Constants::getAssetTypeReadableNameFromString
    (const string& type)
    {
        if (type == ASSET_TYPE_ANIMATION)
        {
            return ASSET_TYPE_ANIMATION_READABLE;
        }

        else if (type == ASSET_TYPE_AUDIO)
        {
            return ASSET_TYPE_AUDIO_READABLE;
        }

        else if (type == ASSET_TYPE_FONT)
        {
            return ASSET_TYPE_FONT_READABLE;
        }

        else if (type == ASSET_TYPE_LIGHT)
        {
            return ASSET_TYPE_LIGHT_READABLE;
        }

        else if (type == ASSET_TYPE_MATERIAL)
        {
            return ASSET_TYPE_MATERIAL_READABLE;
        }

        else if (type == ASSET_TYPE_MODEL)
        {
            return ASSET_TYPE_MODEL_READABLE;
        }

        else if (type == ASSET_TYPE_OBJECT_EMITTER)
        {
            return ASSET_TYPE_OBJECT_EMITTER_READABLE;
        }

        else if (type == ASSET_TYPE_PARTICLE_EMITTER)
        {
            return ASSET_TYPE_PARTICLE_EMITTER_READABLE;
        }

        else if (type == ASSET_TYPE_PATH)
        {
            return ASSET_TYPE_PATH_READABLE;
        }

        else if (type == ASSET_TYPE_PHYSICS_OBJECT)
        {
            return ASSET_TYPE_PHYSICS_OBJECT_READABLE;
        }

        else if (type == ASSET_TYPE_SCRIPT)
        {
            return ASSET_TYPE_SCRIPT_READABLE;
        }

        else if (type == ASSET_TYPE_SCROLLER)
        {
            return ASSET_TYPE_SCROLLER_READABLE;
        }

        else if (type == ASSET_TYPE_SHADER)
        {
            return ASSET_TYPE_SHADER_READABLE;
        }

        else if (type == ASSET_TYPE_TEXTURE)
        {
            return ASSET_TYPE_TEXTURE_READABLE;
        }
        return "";
    }

    string
    Constants::getAssetFormatReadableNameFromString
    (const string& format)
    {
        // Script
        if (format == ASSET_FORMAT_SCRIPT_ANGELSCRIPT)
        {
           return ASSET_FORMAT_SCRIPT_ANGELSCRIPT_READABLE;
        }
        // Model
        else if (format == ASSET_FORMAT_MODEL_ASSIMP)
        {
           return ASSET_FORMAT_MODEL_ASSIMP_READABLE;
        }
        else if (format == ASSET_FORMAT_MODEL_OBJ)
        {
           return ASSET_FORMAT_MODEL_OBJ_READABLE;
        }
        // Audio
        else if (format == ASSET_FORMAT_AUDIO_OGG)
        {
           return ASSET_FORMAT_AUDIO_OGG_READABLE;
        }
        else if (format == ASSET_FORMAT_AUDIO_WAV)
        {
           return ASSET_FORMAT_AUDIO_WAV_READABLE;
        }
        // Font
        else if (format == ASSET_FORMAT_FONT_TTF)
        {
           return ASSET_FORMAT_FONT_TTF_READABLE;
        }
        // Shader
        else if (format == ASSET_FORMAT_SHADER_GLSL)
        {
           return ASSET_FORMAT_SHADER_GLSL_READABLE;
        }
        // Light
        else if (format == ASSET_FORMAT_LIGHT_POINT)
        {
           return ASSET_FORMAT_LIGHT_POINT_READABLE;
        }
        else if (format == ASSET_FORMAT_LIGHT_DIRECTIONAL)
        {
           return ASSET_FORMAT_LIGHT_DIRECTIONAL_READABLE;
        }
        else if (format == ASSET_FORMAT_LIGHT_SPOTLIGHT)
        {
           return ASSET_FORMAT_LIGHT_SPOTLIGHT_READABLE;
        }
        // Dream
        else if (format == ASSET_FORMAT_DREAM)
        {
            return ASSET_FORMAT_DREAM_READABLE;
        }
        // Physics
        else if (format == COLLISION_SHAPE_SPHERE)
        {
            return COLLISION_SHAPE_SPHERE_READABLE;
        }
        else if (format == COLLISION_SHAPE_BOX)
        {
            return COLLISION_SHAPE_BOX_READABLE;
        }
        else if (format == COLLISION_SHAPE_CYLINDER)
        {
            return COLLISION_SHAPE_CYLINDER_READABLE;
        }
        else if (format == COLLISION_SHAPE_CAPSULE)
        {
            return COLLISION_SHAPE_CAPSULE_READABLE;
        }
        else if (format == COLLISION_SHAPE_CONE)
        {
            return COLLISION_SHAPE_MULTI_SPHERE_READABLE;
        }
        else if (format == COLLISION_SHAPE_CONVEX_HULL)
        {
            return COLLISION_SHAPE_CONVEX_HULL_READABLE;
        }
        else if (format == COLLISION_SHAPE_CONVEX_TRIANGLE_MESH)
        {
            return COLLISION_SHAPE_CONVEX_TRIANGLE_MESH_READABLE;
        }
        else if (format == COLLISION_SHAPE_BVH_TRIANGLE_MESH)
        {
            return COLLISION_SHAPE_BVH_TRIANGLE_MESH_READABLE;
        }
        else if (format == COLLISION_SHAPE_HEIGHTFIELD_TERRAIN)
        {
            return COLLISION_SHAPE_HEIGHTFIELD_TERRAIN_READABLE;
        }
        else if (format == COLLISION_SHAPE_STATIC_PLANE)
        {
            return COLLISION_SHAPE_STATIC_PLANE_READABLE;
        }
        else if (format == COLLISION_SHAPE_COMPOUND)
        {
            return COLLISION_SHAPE_COMPOUND_READABLE;
        }
        else if (format == COLLISION_SHAPE_MULTI_SPHERE)
        {
            return COLLISION_SHAPE_MULTI_SPHERE_READABLE;
        }

        return "";
    }

    string
    Constants::getAssetFormatStringFromReadableName
    (const string& format)
    {
        if (format == ASSET_FORMAT_SCRIPT_ANGELSCRIPT_READABLE)
        {
           return ASSET_FORMAT_SCRIPT_ANGELSCRIPT;
        }
        else if (format == ASSET_FORMAT_MODEL_ASSIMP_READABLE)
        {
           return ASSET_FORMAT_MODEL_ASSIMP;
        }
        else if (format == ASSET_FORMAT_MODEL_OBJ_READABLE)
        {
           return ASSET_FORMAT_MODEL_OBJ;
        }
        else if (format == ASSET_FORMAT_AUDIO_OGG_READABLE)
        {
           return ASSET_FORMAT_AUDIO_OGG;
        }
        else if (format == ASSET_FORMAT_AUDIO_WAV_READABLE)
        {
           return ASSET_FORMAT_AUDIO_WAV;
        }
        else if (format == ASSET_FORMAT_FONT_TTF_READABLE)
        {
           return ASSET_FORMAT_FONT_TTF;
        }
        else if (format == ASSET_FORMAT_SHADER_GLSL_READABLE)
        {
           return ASSET_FORMAT_SHADER_GLSL;
        }
        else if (format == ASSET_FORMAT_LIGHT_POINT_READABLE)
        {
           return ASSET_FORMAT_LIGHT_POINT;
        }
        else if (format == ASSET_FORMAT_LIGHT_DIRECTIONAL_READABLE)
        {
           return ASSET_FORMAT_LIGHT_DIRECTIONAL;
        }
        else if (format == ASSET_FORMAT_LIGHT_SPOTLIGHT_READABLE)
        {
           return ASSET_FORMAT_LIGHT_SPOTLIGHT;
        }
        else if (format == ASSET_FORMAT_DREAM_READABLE)
        {
            return ASSET_FORMAT_DREAM;
        }
        else if (format == COLLISION_SHAPE_SPHERE_READABLE)
        {
            return COLLISION_SHAPE_SPHERE;
        }
        else if (format == COLLISION_SHAPE_BOX_READABLE)
        {
            return COLLISION_SHAPE_BOX;
        }
        else if (format == COLLISION_SHAPE_CYLINDER_READABLE)
        {
            return COLLISION_SHAPE_CYLINDER;
        }
        else if (format == COLLISION_SHAPE_CAPSULE_READABLE)
        {
            return COLLISION_SHAPE_CAPSULE;
        }
        else if (format == COLLISION_SHAPE_CONE_READABLE)
        {
            return COLLISION_SHAPE_CONE;
        }
        else if (format == COLLISION_SHAPE_CONVEX_HULL_READABLE)
        {
            return COLLISION_SHAPE_CONVEX_HULL;
        }
        else if (format == COLLISION_SHAPE_CONVEX_TRIANGLE_MESH_READABLE)
        {
            return COLLISION_SHAPE_CONVEX_TRIANGLE_MESH;
        }
        else if (format == COLLISION_SHAPE_BVH_TRIANGLE_MESH_READABLE)
        {
            return COLLISION_SHAPE_BVH_TRIANGLE_MESH;
        }
        else if (format == COLLISION_SHAPE_HEIGHTFIELD_TERRAIN_READABLE)
        {
            return COLLISION_SHAPE_HEIGHTFIELD_TERRAIN;
        }
        else if (format == COLLISION_SHAPE_STATIC_PLANE_READABLE)
        {
            return COLLISION_SHAPE_STATIC_PLANE;
        }
        else if (format == COLLISION_SHAPE_COMPOUND_READABLE)
        {
            return COLLISION_SHAPE_COMPOUND;
        }
        else if (format == COLLISION_SHAPE_MULTI_SPHERE_READABLE)
        {
            return COLLISION_SHAPE_MULTI_SPHERE;
        }

        return "";
    }

    vector<string> Constants::DREAM_PATH_SPLINE_TYPES
    {
        DREAM_PATH_TYPE_OPEN,
        DREAM_PATH_TYPE_BEZIER,
        DREAM_PATH_TYPE_CLAMPED
    };

}


