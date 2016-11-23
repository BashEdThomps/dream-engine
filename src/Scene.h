/*
* Dream::Scene
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>

#include "SceneObject.h"
#include "Components/Video/Camera.h"
#include "json.hpp"
#include "String.h"

#define SCENE_JSON_UUID              "uuid"
#define SCENE_JSON_NAME              "name"
#define SCENE_JSON_ANIMATION_ENABLED "animationEnabled"
#define SCENE_JSON_AUDIO_ENABLED     "audioEnabled"
#define SCENE_JSON_PHYSICS_ENABLED   "physicsEnabled"
#define SCENE_JSON_INPUT_ENABLED     "inputEnabled"
#define SCENE_JSON_SCRIPTING_ENABLED "scriptingEnabled"
#define SCENE_JSON_SCENE_OBJECTS     "objects"
#define SCENE_JSON_CAMERA            "camera"
#define SCENE_JSON_TRANSLATION       "translation"
#define SCENE_JSON_ROTATION          "rotation"
#define SCENE_JSON_MOVEMENT_SPEED    "movementSpeed"

#define SCENE_JSON_X "x"
#define SCENE_JSON_Y "y"
#define SCENE_JSON_Z "z"

namespace Dream {
  class Scene {
  public:
    std::string mUUID;
    std::string mName;
    bool mAnimationEnabled;
    bool mAudioEnabled;
    bool mInputEnabled;
    bool mPhysicsEnabled;
    bool mScriptingEnabled;
    SceneObject *mRootSceneObject;
    std::vector<SceneObject*> mScenegraphVector;
    Transform3D* mDefaultCameraTransform;
    float mCameraMovementSpeed;
  public:
    Scene();
    Scene(nlohmann::json);
    ~Scene();
    bool         init();
    std::string  getUUID();
    void         setUUID(std::string);
    std::string  getName();
    void         setName(std::string);
    void         loadSceneObjects(nlohmann::json,SceneObject*);
    void         loadDefaultCameraTransform(nlohmann::json);
    int          countChildrenOfSceneObject(SceneObject*);
    void         setRootSceneObject(SceneObject*);
    SceneObject* getRootSceneObject();
    size_t          getNumberOfSceneObjects();
    bool         hasSceneObect(SceneObject*);
    SceneObject* getSceneObjectByName(std::string);
    SceneObject* getSceneObjectByUUID(std::string);
    std::string  generateSceneObjectPath(SceneObject*);
    void         showStatus();
    void         showScenegraph();
    std::string  indent(int);
    void         generateScenegraphVector();
    void         generateSceneObjectPaths();
    bool         isScenegraphVectorEmpty();
    std::vector<SceneObject*> getScenegraphVector();

    std::vector<float> getDefaultCameraTranslation();
    std::vector<float> getDefaultCameraRotation();

    void setCameraMovementSpeed(float);
    float getCameraMovementSpeed();

  }; // End of Scene

} // End of Dream

#endif // End of SCENE_H
