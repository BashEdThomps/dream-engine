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
  namespace Components {
    namespace Scripting {

      ChaiScriptAPI::ChaiScriptAPI(Dream* dream)  {
        mDream = dream;
      }

      ChaiScriptAPI::~ChaiScriptAPI(void) {}

      bool ChaiScriptAPI::init() {
        std::cout << "ChaiScriptAPI: Initialising..." << std::endl;
        mEngine = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());
        initAPIs();
        std::cout << "done!" << std::endl;
        return true;
      }

      bool ChaiScriptAPI::loadScript(ChaiScriptInstance* script) {
        std::string scriptPath = script->getAbsolutePath();
        std::cout << "ChaiScriptAPI: Loading from " << scriptPath << std::endl;
        chaiscript::Boxed_Value result = mEngine->eval_file(scriptPath);
        return !result.is_null();
      }

      void ChaiScriptAPI::update(Scene* scene) {
        handleInputs(scene->getRootSceneObject());
        std::vector<SceneObject*> scenegraph = scene->getScenegraphVector();
        std::vector<SceneObject*>::iterator sgIter;
        for (sgIter = scenegraph.begin(); sgIter != scenegraph.end(); sgIter++) {
          SceneObject *currentSceneObject = (*sgIter);
          if (currentSceneObject->hasScriptAssetInstance()) {
            try {
              ChaiScriptInstance* script;
              script = dynamic_cast<ChaiScriptInstance*>(currentSceneObject->getScriptAssetInstance());
              script->update();
            } catch (const std::exception &ex) {
              std::cerr << "ChaiScriptAPI: Exception Running Update on "
                        << currentSceneObject->getNameUUIDString() << std::endl << ex.what() << std::endl;
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

      bool ChaiScriptAPI::importScriptAssetByUUID(std::string uuid) {
        AssetDefinition* scriptAsset;
        scriptAsset = mDream->getAssetManager()->getAssetDefinitionByUUID(uuid);
        std::cout << "ChaiAssetInstance: Importing ChaiScript Library: " << scriptAsset->getNameAndUUIDString() << std::endl;
        chaiscript::Boxed_Value result = mEngine->eval_file(mDream->getProject()->getProjectPath()+scriptAsset->getAssetPath());
        return !result.is_null();
      }

      void ChaiScriptAPI::initAssetInstanceAPI() {
        std::cout << "ChaiScriptInstance: Init AssetInstance API" << std::endl;
        mEngine->add(chaiscript::user_type<AssetInstance>(),"AssetInstance");
      }

      void ChaiScriptAPI::initProjectAPI() {
        std::cout << "ChaiScriptInstance: Init Project API" << std::endl;
        mEngine->add(chaiscript::user_type<Project>(),"Project");
        mEngine->add(chaiscript::fun(&Dream::getProject),"getProject");
        mEngine->add(chaiscript::fun(&Dream::getAssetManager),"getAssetManager");
      }

      void ChaiScriptAPI::initAssetManagerAPI() {
        std::cout << "ChaiScriptInstance: Init AssetManager API" << std::endl;
        mEngine->add(chaiscript::user_type<AssetManager>(),"AssetManager");
        mEngine->add(chaiscript::fun(&AssetManager::getAssetInstanceByUUID),"getAssetInstanceByUUID");
      }

      void ChaiScriptAPI::initAudioComponentAPI() {
        std::cout << "ChaiScriptInstance: Init AudioComponent API" << std::endl;
        mEngine->add(chaiscript::user_type<Components::Audio::AudioComponent>(),"AudioComponent");
        mEngine->add(chaiscript::fun(&Components::Audio::AudioComponent::playAsset),"playAsset");
        mEngine->add(chaiscript::fun(&Components::Audio::AudioComponent::pauseAsset),"pauseAsset");
        mEngine->add(chaiscript::fun(&Components::Audio::AudioComponent::stopAsset), "stopAsset");
      }

      void ChaiScriptAPI::initComponentManagerAPI() {
        mEngine->add(chaiscript::fun(&Dream::getCamera), "getCamera");
        mEngine->add(chaiscript::fun(&Dream::getAnimationComponent), "getAnimationComponent");
        mEngine->add(chaiscript::fun(&Dream::getAudioComponent), "getAudioComponent");
        mEngine->add(chaiscript::fun(&Dream::getPhysicsComponent), "getPhysicsComponent");
        mEngine->add(chaiscript::fun(&Dream::getGraphicsComponent), "getGraphicsComponent");
      }

      void ChaiScriptAPI::initTimeAPI() {
        std::cout << "ChaiScriptInstance: Init Time API" << std::endl;
        mEngine->add(chaiscript::user_type<Time>(),"Time");
        mEngine->add(chaiscript::fun(&Dream::getTime),"getTime");
        mEngine->add(chaiscript::fun(&Time::update),"update");
        mEngine->add(chaiscript::fun(&Time::getCurrentTime),"getCurrentTime");
        mEngine->add(chaiscript::fun(&Time::getLastTime),"getLastTime");
        mEngine->add(chaiscript::fun(&Time::getTimeDelta),"getTimeDelta");
      }

      void ChaiScriptAPI::initCameraAPI() {
        std::cout << "ChaiScriptInstance: Init Camera API" << std::endl;
        // Class Definition
        mEngine->add(chaiscript::user_type<Graphics::Camera>(),"Camera");
        // Methods
        mEngine->add(chaiscript::fun(&Graphics::Camera::processMouseScroll),"processMouseScroll");
        mEngine->add(chaiscript::fun(&Graphics::Camera::processKeyboard),"processKeyboard");
        mEngine->add(chaiscript::fun(&Graphics::Camera::processMouseMovement),"processMouseMovement");
        mEngine->add(chaiscript::fun(&Graphics::Camera::getMouseSensitivity),"getMouseSensitivity");
        mEngine->add(chaiscript::fun(&Graphics::Camera::setMouseSensitivity),"setMouseSensitivity");
        mEngine->add(chaiscript::fun(&Graphics::Camera::getMovementSpeed),"getMovementSpeed");
        mEngine->add(chaiscript::fun(&Graphics::Camera::setMovementSpeed),"setMovementSpeed");
        // Global Variables
        mEngine->add_global(chaiscript::const_var(CAMERA_MOVEMENT_FORWARD), "CAMERA_MOVEMENT_FORWARD");
        mEngine->add_global(chaiscript::const_var(CAMERA_MOVEMENT_BACKWARD),"CAMERA_MOVEMENT_BACKWARD");
        mEngine->add_global(chaiscript::const_var(CAMERA_MOVEMENT_LEFT), "CAMERA_MOVEMENT_LEFT");
        mEngine->add_global(chaiscript::const_var(CAMERA_MOVEMENT_RIGHT), "CAMERA_MOVEMENT_RIGHT");
      }

      void ChaiScriptAPI::initSceneObjectAPI() {
        std::cout << "ChaiScriptInstance: Init SceneObject API" << std::endl;
        mEngine->add(chaiscript::user_type<SceneObject>(), "SceneObject");
        mEngine->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<float>>("FloatVector"));
        mEngine->add(chaiscript::fun(&SceneObject::getAnimationAssetInstance),"getAnimationAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getAudioAssetInstance),"getAudioAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getLightAssetInstance),"getLightAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getModelAssetInstance),"getModelAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getShaderAssetInstance),"getShaderAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getScriptAssetInstance),"getScriptAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getPhysicsObjectAssetInstance),"getPhysicsObjectAsset");
        mEngine->add(chaiscript::fun(&SceneObject::getTranslation),"getTranslation");
        mEngine->add(chaiscript::fun<void,SceneObject,float,float,float>(&SceneObject::setTranslation),"setTranslation");
        mEngine->add(chaiscript::fun<void,SceneObject,std::vector<float>>(&SceneObject::setTranslation),"setTranslation");
        mEngine->add(chaiscript::fun(&SceneObject::getRotation),"getRotation");
        mEngine->add(chaiscript::fun<void, SceneObject, float, float, float>(&SceneObject::setRotation),"setRotation");
        mEngine->add(chaiscript::fun<void,SceneObject,std::vector<float>>(&SceneObject::setRotation),"setRotation");
        mEngine->add(chaiscript::fun(&SceneObject::getScale),"getScale");
        mEngine->add(chaiscript::fun<void,SceneObject,float,float,float>(&SceneObject::setScale),"setScale");
        mEngine->add(chaiscript::fun<void,SceneObject,std::vector<float>>(&SceneObject::setScale),"setScale");
      }

      void ChaiScriptAPI::initGraphicsComponentAPI() {
        mEngine->add(chaiscript::user_type<Components::Graphics::GraphicsComponent>(),"GraphicsComponent");
        mEngine->add(chaiscript::fun(&Components::Graphics::GraphicsComponent::closeWindow),"closeWindow");
        mEngine->add(chaiscript::fun(&Components::Graphics::GraphicsComponent::setCursorEnabled),"setCursorEnabled");
      }

      void ChaiScriptAPI::initAnimationInstanceAPI() {
        mEngine->add(chaiscript::user_type<Components::Animation::AnimationInstance>(),"AnimationInstance");
        mEngine->add(chaiscript::fun(&Components::Animation::AnimationInstance::isLooping),"isLooping");
        mEngine->add(chaiscript::fun(&Components::Animation::AnimationInstance::setLooping),"setLooping");
      }

      void ChaiScriptAPI::initAnimationComponentAPI() {
        mEngine->add(chaiscript::user_type<Components::Animation::AnimationComponent>(),"AnimationComponent");
        mEngine->add(chaiscript::fun(&Components::Animation::AnimationComponent::play),"play");
        mEngine->add(chaiscript::fun(&Components::Animation::AnimationComponent::pause),"pause");
        mEngine->add(chaiscript::fun(&Components::Animation::AnimationComponent::stop),"stop");
        mEngine->add(chaiscript::fun(&Components::Animation::AnimationComponent::isLooping),"isLooping");
        mEngine->add(chaiscript::fun(&Components::Animation::AnimationComponent::setLooping),"setLooping");
      }

      void ChaiScriptAPI::initSceneAPI() {
        mEngine->add(chaiscript::user_type<Scene>(),"Scene");
        mEngine->add(chaiscript::fun(&Project::getActiveScene),"getScene");
        mEngine->add(chaiscript::fun(&Scene::getSceneObjectByUUID),"getSceneObjectByUUID");
      }

    } // End of Scripting
  } // End of Components
} // End of Dream
