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

// Enable Output =================================================================
    //#define DEBUG false
    #define DEBUG true
// Misc ==========================================================================
    #define DIR_PATH_SEP "/"
    #define DREAM_TOOL_DIR ".dreamtool"
    #define PATH_DELIMETER "::"
// Project =======================================================================
    #define PROJECT_DIRECTORY_ARG "--project-dir"
    #define PROJECT_UUID_ARG "--uuid"
    #define PROJECT_UUID_LENGTH 19
    #define PROJECT_EXTENSION ".json"
    #define PROJECT_PATH_SEP "/"
    #define PROJECT_UUID "uuid"
    #define PROJECT_NAME "name"
    #define PROJECT_AUTHOR "author"
    #define PROJECT_DESCRIPTION "description"
    #define PROJECT_SCENE_ARRAY "scenes"
    #define PROJECT_ASSET_ARRAY "assets"
    #define PROJECT_STARTUP_SCENE "startupScene"
    #define PROJECT_WINDOW_SIZE  "windowSize"
    #define PROJECT_WIDTH "width"
    #define PROJECT_HEIGHT "height"
// Asset =========================================================================
    #define ASSET_UUID "uuid"
    #define ASSET_NAME "name"
    #define ASSET_TYPE "type"
    #define ASSET_FORMAT "format"
    #define ASSET_DIR "assets"
    #define ASSET_DIR_ANIMATION "animation"
    #define ASSET_DIR_AUDIO "audio"
    #define ASSET_DIR_MODEL "model"
    #define ASSET_DIR_SCRIPT "script"
    #define ASSET_DIR_SHADER "shader"
    #define ASSET_DIR_SPRITE "sprite"
    #define ASSET_DIR_FONT "font"
    #define ASSET_TYPE_ANIMATION "animation"
    #define ASSET_TYPE_AUDIO "audio"
    #define ASSET_TYPE_LIGHT "light"
    #define ASSET_TYPE_FONT "font"
    #define ASSET_TYPE_MODEL "model"
    #define ASSET_TYPE_PHYSICS_OBJECT "physicsObject"
    #define ASSET_TYPE_SCRIPT "script"
    #define ASSET_TYPE_SHADER "shader"
    #define ASSET_TYPE_SPRITE "sprite"
    #define ASSET_DEF_FORMAT_SCRIPT_LUA "lua"
    #define ASSET_DEF_FORMAT_MODEL_ASSIMP "assimp"
    #define ASSET_DEF_FORMAT_AUDIO_MUSIC "music"
    #define ASSET_DEF_FORMAT_AUDIO_SFX "sfx"
    #define ASSET_DEF_FORMAT_ANIMATION_DREAM "dream"
    #define ASSET_ATTR_X "x"
    #define ASSET_ATTR_Y "y"
    #define ASSET_ATTR_Z "z"
// Scene =========================================================================
    #define SCENE_JSON_UUID "uuid"
    #define SCENE_JSON_NAME "name"
    #define SCENE_JSON_SCENE_OBJECTS "objects"
    #define SCENE_JSON_CAMERA "camera"
    #define SCENE_JSON_TRANSLATION "translation"
    #define SCENE_JSON_ROTATION "rotation"
    #define SCENE_JSON_MOVEMENT_SPEED "movementSpeed"
    #define SCENE_JSON_X "x"
    #define SCENE_JSON_Y "y"
    #define SCENE_JSON_Z "z"
    #define SCENE_JSON_CLEAR_COLOUR "clearColour"
    #define SCENE_JSON_RED "r"
    #define SCENE_JSON_GREEN "g"
    #define SCENE_JSON_BLUE "b"
    #define SCENE_JSON_ALPHA "a"
// SceneObject ===================================================================
    #define SO_X 0
    #define SO_Y 1
    #define SO_Z 2
    #define SCENE_OBJECT_UUID "uuid"
    #define SCENE_OBJECT_NAME "name"
    #define SCENE_OBJECT_CHILDREN "children"
    #define SCENE_OBJECT_HAS_FOCUS "hasFocus"
    #define SCENE_OBJECT_TRANSLATION "translation"
    #define SCENE_OBJECT_ROTATION "rotation"
    #define SCENE_OBJECT_SCALE "scale"
    #define SCENE_OBJECT_X "x"
    #define SCENE_OBJECT_Y "y"
    #define SCENE_OBJECT_Z "z"
    #define SCENE_OBJECT_TRANSFORM_TYPE "transformType"
    #define SCENE_OBJECT_TRANSFORM_TYPE_OFFSET "offset"
    #define SCENE_OBJECT_TRANSFORM_TYPE_ABSOLUTE "absolute"
    #define SCENE_OBJECT_ASSET_INSTANCES "assetInstances"
// Transform =====================================================================
    #define TRANSFORM_X 0
    #define TRANSFORM_Y 1
    #define TRANSFORM_Z 2
    #define TRANSFORM_TYPE_ABSOLUTE "absolute"
    #define TRANSFORM_TYPE_OFFSET "offset"
// Font ==========================================================================
    #define FONT_SIZE "size"
    #define FONT_COLOUR "colour"
    #define FONT_RED    "r"
    #define FONT_GREEN  "g"
    #define FONT_BLUE   "b"
// Animation =====================================================================
    #define ASSET_FORMAT_DREAM_ANIMATION "dream"
    #define ASSET_ATTR_KEYFRAMES     "keyframes"
    #define ASSET_ATTR_WRAP          "wrap"
    #define ASSET_ATTR_TRANSLATION   "translation"
    #define ASSET_ATTR_ROTATION      "rotation"
    #define ASSET_ATTR_SCALE         "scale"
    #define ASSET_ATTR_START_TIME    "startTime"
    #define ASSET_ATTR_WRAP          "wrap"
    #define ASSET_ATTR_LOOP          "loop"
    #define ASSET_ATTR_INTERPOLATION "interpolation"
    #define DREAM_ANIMATION_STOPPED 0
    #define DREAM_ANIMATION_PLAYING 1
    #define DREAM_ANIMATION_PAUSED  2
    #define MAX_FRAME_ADVANCE  10
    #define DREAM_ANIMATION_INTERPOLATION_NONE   "none"
    #define DREAM_ANIMATION_INTERPOLATION_LINEAR "linear"
    #define DREAM_ANIMATION_INTERPOLATION_BEZIER "bezier"
// Sprite ========================================================================
    #define SPRITE_TILE_SIZE "tileSize"
    #define SPRITE_TILE_WIDTH "width"
    #define SPRITE_TILE_HEIGHT "height"




#endif // CONSTANTS_H
