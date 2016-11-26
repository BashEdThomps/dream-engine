/*
 * ChaiScriptAPI
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

#include "ChaiScriptAPI.h"

namespace Dream {

      ChaiScriptAPI::ChaiScriptAPI(Dream* dream)  {
        mDream = dream;
      }

      ChaiScriptAPI::~ChaiScriptAPI(void) {}

      bool ChaiScriptAPI::init() {
        cout << "ChaiScriptAPI: Initialising..." << endl;
        mEngine = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());
        initAPIs();
        cout << "done!" << endl;
        return true;
      }

      bool ChaiScriptAPI::loadScript(ChaiScriptInstance* script) {
        string scriptPath = script->getAbsolutePath();
        cout << "ChaiScriptAPI: Loading from " << scriptPath << endl;
        chaiscript::Boxed_Value result = mEngine->eval_file(scriptPath);
        return !result.is_null();
      }

      void ChaiScriptAPI::update(Scene* scene) {
        handleInputs(scene->getRootSceneObject());
        vector<SceneObject*> scenegraph = scene->getScenegraphVector();
        vector<SceneObject*>::iterator sgIter;
        for (sgIter = scenegraph.begin(); sgIter != scenegraph.end(); sgIter++) {
          SceneObject *currentSceneObject = (*sgIter);
          if (currentSceneObject->hasScriptInstance()) {
            try {
              ChaiScriptInstance* script;
              script = dynamic_cast<ChaiScriptInstance*>(currentSceneObject->getScriptInstance());
              script->update();
            } catch (const exception &ex) {
              cerr << "ChaiScriptAPI: Exception Running Update on "
                        << currentSceneObject->getNameUUIDString() << endl << ex.what() << endl;
            }
          }
        }
      }

      void ChaiScriptAPI::handleInputs(SceneObject* root) {
        // TODO - This
        // Pass...
      }

      void ChaiScriptAPI::initAPIs() {
        initAssetInstanceAPI();
        initAnimationInstanceAPI();
        initAnimationComponentAPI();
        initAssetManagerAPI();
        initAudioComponentAPI();
        initCameraAPI();
        initTimeAPI();
        initComponentManagerAPI();
        initProjectAPI();
        initSceneObjectAPI();
        initSceneAPI();
        initGraphicsComponentAPI();
      }

      bool ChaiScriptAPI::importScriptAssetByUUID(string uuid) {
        AssetDefinition* scriptAsset;
        scriptAsset = mDream->getAssetManager()->getAssetDefinitionByUUID(uuid);
        cout << "ChaiAssetInstance: Importing ChaiScript Library: " << scriptAsset->getNameAndUUIDString() << endl;
        chaiscript::Boxed_Value result = mEngine->eval_file(mDream->getProject()->getProjectPath()+scriptAsset->getAssetPath());
        return !result.is_null();
      }

      void ChaiScriptAPI::initAssetInstanceAPI() {
        cout << "ChaiScriptInstance: Init AssetInstance API" << endl;
        mEngine->add(chaiscript::user_type<AssetInstance>(),"AssetInstance");
      }

      void ChaiScriptAPI::initProjectAPI() {
        cout << "ChaiScriptInstance: Init Project API" << endl;
        mEngine->add(chaiscript::user_type<Project>(),"Project");
        mEngine->add(chaiscript::fun(&Dream::getProject),"getProject");
        mEngine->add(chaiscript::fun(&Dream::getAssetManager),"getAssetManager");
      }

      void ChaiScriptAPI::initAssetManagerAPI() {
        cout << "ChaiScriptInstance: Init AssetManager API" << endl;
        mEngine->add(chaiscript::user_type<AssetManager>(),"AssetManager");
        mEngine->add(chaiscript::fun(&AssetManager::getAssetInstanceByUUID),"getAssetInstanceByUUID");
      }

      void ChaiScriptAPI::initAudioComponentAPI() {
        cout << "ChaiScriptInstance: Init AudioComponent API" << endl;
        mEngine->add(chaiscript::user_type<AudioComponent>(),"AudioComponent");
        mEngine->add(chaiscript::fun(&AudioComponent::playAsset),"playAsset");
        mEngine->add(chaiscript::fun(&AudioComponent::pauseAsset),"pauseAsset");
        mEngine->add(chaiscript::fun(&AudioComponent::stopAsset), "stopAsset");
      }

      void ChaiScriptAPI::initComponentManagerAPI() {
        mEngine->add(chaiscript::fun(&Dream::getCamera), "getCamera");
        mEngine->add(chaiscript::fun(&Dream::getAnimationComponent), "getAnimationComponent");
        mEngine->add(chaiscript::fun(&Dream::getAudioComponent), "getAudioComponent");
        mEngine->add(chaiscript::fun(&Dream::getPhysicsComponent), "getPhysicsComponent");
        mEngine->add(chaiscript::fun(&Dream::getGraphicsComponent), "getGraphicsComponent");
      }

      void ChaiScriptAPI::initTimeAPI() {
        cout << "ChaiScriptInstance: Init Time API" << endl;
        mEngine->add(chaiscript::user_type<Time>(),"Time");
        mEngine->add(chaiscript::fun(&Dream::getTime),"getTime");
        mEngine->add(chaiscript::fun(&Time::update),"update");
        mEngine->add(chaiscript::fun(&Time::getCurrentTime),"getCurrentTime");
        mEngine->add(chaiscript::fun(&Time::getLastTime),"getLastTime");
        mEngine->add(chaiscript::fun(&Time::getTimeDelta),"getTimeDelta");
      }

      void ChaiScriptAPI::initCameraAPI() {
        cout << "ChaiScriptInstance: Init Camera API" << endl;
        // Class Definition
        mEngine->add(chaiscript::user_type<Camera>(),"Camera");
        // Methods
        mEngine->add(chaiscript::fun(&Camera::processMouseScroll),"processMouseScroll");
        mEngine->add(chaiscript::fun(&Camera::processKeyboard),"processKeyboard");
        mEngine->add(chaiscript::fun(&Camera::processMouseMovement),"processMouseMovement");
        mEngine->add(chaiscript::fun(&Camera::getMouseSensitivity),"getMouseSensitivity");
        mEngine->add(chaiscript::fun(&Camera::setMouseSensitivity),"setMouseSensitivity");
        mEngine->add(chaiscript::fun(&Camera::getMovementSpeed),"getMovementSpeed");
        mEngine->add(chaiscript::fun(&Camera::setMovementSpeed),"setMovementSpeed");
        // Global Variables
        mEngine->add_global(chaiscript::const_var(CAMERA_MOVEMENT_FORWARD), "CAMERA_MOVEMENT_FORWARD");
        mEngine->add_global(chaiscript::const_var(CAMERA_MOVEMENT_BACKWARD),"CAMERA_MOVEMENT_BACKWARD");
        mEngine->add_global(chaiscript::const_var(CAMERA_MOVEMENT_LEFT), "CAMERA_MOVEMENT_LEFT");
        mEngine->add_global(chaiscript::const_var(CAMERA_MOVEMENT_RIGHT), "CAMERA_MOVEMENT_RIGHT");
      }

      void ChaiScriptAPI::initSceneObjectAPI() {
        cout << "ChaiScriptInstance: Init SceneObject API" << endl;
        mEngine->add(chaiscript::user_type<SceneObject>(), "SceneObject");
        mEngine->add(chaiscript::bootstrap::standard_library::vector_type<vector<float>>("FloatVector"));
        mEngine->add(chaiscript::fun(&SceneObject::getAnimationInstance),"getAnimationAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getAudioInstance),"getAudioAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getLightInstance),"getLightAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getModelInstance),"getModelAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getShaderInstance),"getShaderAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getScriptInstance),"getScriptAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getPhysicsObjectInstance),"getPhysicsObjectAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getSpriteInstance),"getSpriteAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getTranslation),"getTranslation");
        mEngine->add(chaiscript::fun<void,SceneObject,float,float,float>(&SceneObject::setTranslation),"setTranslation");
        mEngine->add(chaiscript::fun<void,SceneObject,vector<float>>(&SceneObject::setTranslation),"setTranslation");
        mEngine->add(chaiscript::fun(&SceneObject::getRotation),"getRotation");
        mEngine->add(chaiscript::fun<void, SceneObject, float, float, float>(&SceneObject::setRotation),"setRotation");
        mEngine->add(chaiscript::fun<void,SceneObject,vector<float>>(&SceneObject::setRotation),"setRotation");
        mEngine->add(chaiscript::fun(&SceneObject::getScale),"getScale");
        mEngine->add(chaiscript::fun<void,SceneObject,float,float,float>(&SceneObject::setScale),"setScale");
        mEngine->add(chaiscript::fun<void,SceneObject,vector<float>>(&SceneObject::setScale),"setScale");
      }

      void ChaiScriptAPI::initGraphicsComponentAPI() {
        mEngine->add(chaiscript::user_type<GraphicsComponent>(),"GraphicsComponent");
        mEngine->add(chaiscript::fun(&GraphicsComponent::closeWindow),"closeWindow");
        mEngine->add(chaiscript::fun(&GraphicsComponent::setCursorEnabled),"setCursorEnabled");
      }

      void ChaiScriptAPI::initAnimationInstanceAPI() {
        mEngine->add(chaiscript::user_type<AnimationInstance>(),"AnimationInstance");
        mEngine->add(chaiscript::fun(&AnimationInstance::isLooping),"isLooping");
        mEngine->add(chaiscript::fun(&AnimationInstance::setLooping),"setLooping");
      }

      void ChaiScriptAPI::initAnimationComponentAPI() {
        mEngine->add(chaiscript::user_type<AnimationComponent>(),"AnimationComponent");
        mEngine->add(chaiscript::fun(&AnimationComponent::play),"play");
        mEngine->add(chaiscript::fun(&AnimationComponent::pause),"pause");
        mEngine->add(chaiscript::fun(&AnimationComponent::stop),"stop");
        mEngine->add(chaiscript::fun(&AnimationComponent::isLooping),"isLooping");
        mEngine->add(chaiscript::fun(&AnimationComponent::setLooping),"setLooping");
      }

      void ChaiScriptAPI::initSceneAPI() {
        mEngine->add(chaiscript::user_type<Scene>(),"Scene");
        mEngine->add(chaiscript::fun(&Project::getActiveScene),"getScene");
        mEngine->add(chaiscript::fun(&Scene::getSceneObjectByUUID),"getSceneObjectByUUID");
      }

} // End of Dream
