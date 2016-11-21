/*
 * ChaiScriptInstance
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

#include "ChaiScriptInstance.h"

namespace Dream {
  namespace Components {
    namespace Scripting {

      ChaiScriptInstance::ChaiScriptInstance(AssetDefinition* definition)
        : AssetInstance(definition) {
        setAssetManager(NULL);
      }

      ChaiScriptInstance::~ChaiScriptInstance() {

      }

      bool ChaiScriptInstance::load(std::string projectPath) {
        mProjectPath = projectPath;
        mScript = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());
        initChaiScriptAPI();
        std::string scriptPath = projectPath+mDefinition->getAssetPath();
        std::cout << "ChaiScriptInstance: Loading from " << scriptPath << std::endl;
        chaiscript::Boxed_Value result = mScript->eval_file(scriptPath);
        initAPIs();
        return !result.is_null();
      }

      void ChaiScriptInstance::setAssetManager(AssetManager* assetManager) {
        mAssetManager = assetManager;
      }

      void ChaiScriptInstance::initAPIs() {
        initAssetInstanceAPI();
        initAnimationInstanceAPI();
        initAnimationComponentAPI();
        initAssetManagerAPI();
        initAudioComponentAPI();
        initCameraAPI();
        initGlfwTimeAPI();
        initInputEventAPI();
        initComponentManagerAPI();
        initProjectAPI();
        initSceneObjectAPI();
        initSceneAPI();
        initVideoComponentAPI();
      }

      std::string ChaiScriptInstance::getProjectPath() {
        return mProjectPath;
      }

      void ChaiScriptInstance::initChaiScriptAPI() {
        mScript->add(chaiscript::user_type<ChaiScriptInstance>(),"ChaiScriptInstance");
        mScript->add_global(chaiscript::var(this),"Script");
        mScript->add(chaiscript::fun(&ChaiScriptInstance::importScriptAssetByUUID),"import");
      }

      bool ChaiScriptInstance::importScriptAssetByUUID(std::string uuid) {
        AssetDefinition* scriptAsset;
        scriptAsset = mAssetManager->getAssetDefinitionByUUID(uuid);
        std::cout << "ChaiAssetInstance: Importing ChaiScript Library: " << scriptAsset->getNameAndUUIDString() << std::endl;
        chaiscript::Boxed_Value result = mScript->eval_file(getProjectPath()+scriptAsset->getAssetPath());
        return !result.is_null();
      }

      void ChaiScriptInstance::initAssetInstanceAPI() {
        std::cout << "ChaiScriptInstance: Init AssetInstance API" << std::endl;
        mScript->add(chaiscript::user_type<AssetInstance>(),"AssetInstance");
      }

      void ChaiScriptInstance::initProjectAPI() {
        std::cout << "ChaiScriptInstance: Init Project API" << std::endl;
        mScript->add(chaiscript::user_type<Dream::Project>(),"Project");
        mScript->add(chaiscript::fun(&Dream::getProject),"getProject");
        mScript->add(chaiscript::fun(&Dream::getComponentManager),"getComponentManager");
        mScript->add(chaiscript::fun(&Dream::getAssetManager),"getAssetManager");
      }

      void ChaiScriptInstance::initAssetManagerAPI() {
        std::cout << "ChaiScriptInstance: Init AssetManager API" << std::endl;
        mScript->add(chaiscript::user_type<AssetManager>(),"AssetManager");
        mScript->add(chaiscript::fun(&AssetManager::getAssetInstanceByUUID),"getAssetInstanceByUUID");
      }

      void ChaiScriptInstance::initAudioComponentAPI() {
        std::cout << "ChaiScriptInstance: Init AudioComponent API" << std::endl;
        mScript->add(chaiscript::user_type<Components::Audio::AudioComponentInterface>(),"AudioComponentInterface");
        mScript->add(chaiscript::fun(&Components::Audio::AudioComponentInterface::playAudioAsset),"playAudioAsset");
        mScript->add(chaiscript::fun(&Components::Audio::AudioComponentInterface::pauseAudioAsset),"pauseAudioAsset");
        mScript->add(chaiscript::fun(&Components::Audio::AudioComponentInterface::stopAudioAsset), "stopAudioAsset");
        mScript->add(
              chaiscript::fun<float,Components::Audio::AudioComponentInterface,Asset::AssetInstance*>
              (&Components::Audio::AudioComponentInterface::getSampleOffset),
              "getSampleOffset"
              );
        mScript->add(chaiscript::fun(&Components::Audio::AudioComponentInterface::getAudioBuffer),"getAudioBuffer");
      }

      void ChaiScriptInstance::initComponentManagerAPI() {
        std::cout << "ChaiScriptInstance: Init ComponentManager API" << std::endl;
        mScript->add(chaiscript::user_type<Components::ComponentManager>(), "ComponentManager");
        mScript->add(chaiscript::fun(&Components::ComponentManager::getAnimationComponent), "getAnimationComponent");
        mScript->add(chaiscript::fun(&Components::ComponentManager::getAudioComponent), "getAudioComponent");
        mScript->add(chaiscript::fun(&Components::ComponentManager::getInputComponent), "getInputComponent");
        mScript->add(chaiscript::fun(&Components::ComponentManager::getPhysicsComponent), "getPhysicsComponent");
        mScript->add(chaiscript::fun(&Components::ComponentManager::getVideoComponent), "getVideoComponent");
      }

      void ChaiScriptInstance::initGlfwTimeAPI() {
        std::cout << "ChaiScriptInstance: Init GLFWTime API" << std::endl;
        mScript->add(chaiscript::user_type<Time>(),"Time");
        mScript->add_global(chaiscript::var(mTime),"gTime");
        mScript->add(chaiscript::fun(&Time::update),"update");
        mScript->add(chaiscript::fun(&Time::getCurrentTime),"getCurrentTime");
        mScript->add(chaiscript::fun(&Time::getLastTime),"getLastTime");
        mScript->add(chaiscript::fun(&Time::getTimeDelta),"getTimeDelta");
      }

      void ChaiScriptInstance::initCameraAPI() {
        std::cout << "ChaiScriptInstance: Init Camera API" << std::endl;
        // Class Definition
        mScript->add(chaiscript::user_type<Camera>(),"Camera");
        // Methods
        mScript->add(chaiscript::fun(&Camera::processMouseScroll),"processMouseScroll");
        mScript->add(chaiscript::fun(&Camera::processKeyboard),"processKeyboard");
        mScript->add(chaiscript::fun(&Camera::processMouseMovement),"processMouseMovement");
        mScript->add(chaiscript::fun(&Camera::getMouseSensitivity),"getMouseSensitivity");
        mScript->add(chaiscript::fun(&Camera::setMouseSensitivity),"setMouseSensitivity");
        mScript->add(chaiscript::fun(&Camera::getMovementSpeed),"getMovementSpeed");
        mScript->add(chaiscript::fun(&Camera::setMovementSpeed),"setMovementSpeed");
        // Global Variables
        mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_FORWARD),  "CAMERA_MOVEMENT_FORWARD");
        mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_BACKWARD), "CAMERA_MOVEMENT_BACKWARD");
        mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_LEFT),     "CAMERA_MOVEMENT_LEFT");
        mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_RIGHT),    "CAMERA_MOVEMENT_RIGHT");
        // Camera Instance
        mScript->add_global(chaiscript::var(&sCamera),"gCamera");
      }

      void ChaiScriptInstance::initInputEventAPI() {
        std::cout << "ChaiScriptInstance: Init Input Event API" << std::endl;
        // Class Definition
        mScript->add(chaiscript::user_type<Components::Input::InputEvent>(),"InputEvent");
        // Methods
        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getSource),"getSource");

        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getXPosition),"getXPosition");
        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getYPosition),"getYPosition");

        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getXPositionOffset),"getXPositionOffset");
        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getYPositionOffset),"getYPositionOffset");

        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getXScrollOffset),"getXScrollOffset");
        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getYScrollOffset),"getYScrollOffset");

        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getKey),"getKey");
        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getMods),"getMods");
        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getAction),"getAction");
        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getButton),"getButton");
        mScript->add(chaiscript::fun(&Components::Input::InputEvent::getScancode),"getScancode");
        // Constants
        mScript->add_global(chaiscript::const_var(INPUT_TYPE_JOYSTICK),"INPUT_TYPE_JOYSTICK");
        mScript->add_global(chaiscript::const_var(INPUT_TYPE_KEYBOARD),"INPUT_TYPE_KEYBOARD");
        mScript->add_global(chaiscript::const_var(INPUT_TYPE_MOUSE_BUTTON),"INPUT_TYPE_MOUSE_BUTTON");
        mScript->add_global(chaiscript::const_var(INPUT_TYPE_MOUSE_POSITION),"INPUT_TYPE_MOUSE_POSITION");
        mScript->add_global(chaiscript::const_var(INPUT_TYPE_MOUSE_SCROLL),"INPUT_TYPE_MOUSE_SCROLL");
        // Input Actions
        mScript->add_global(chaiscript::const_var(GLFW_PRESS),"INPUT_PRESS");
        mScript->add_global(chaiscript::const_var(GLFW_RELEASE),"INPUT_RELEASE");
        mScript->add_global(chaiscript::const_var(GLFW_REPEAT),"INPUT_REPEAT");
        // Keys
        mScript->add_global(chaiscript::const_var(GLFW_KEY_SPACE),"KEY_SPACE");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_UP),"KEY_UP");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_DOWN),"KEY_DOWN");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_LEFT),"KEY_LEFT");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_RIGHT),"KEY_RIGHT");
        // Char Keys
        mScript->add_global(chaiscript::const_var(GLFW_KEY_A),"KEY_A");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_B),"KEY_B");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_C),"KEY_C");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_D),"KEY_D");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_E),"KEY_E");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_F),"KEY_F");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_G),"KEY_G");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_H),"KEY_H");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_I),"KEY_I");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_J),"KEY_J");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_K),"KEY_K");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_L),"KEY_L");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_M),"KEY_M");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_N),"KEY_N");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_O),"KEY_O");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_P),"KEY_P");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_Q),"KEY_Q");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_R),"KEY_R");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_S),"KEY_S");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_T),"KEY_T");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_U),"KEY_U");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_V),"KEY_V");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_W),"KEY_W");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_X),"KEY_X");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_Y),"KEY_Y");
        mScript->add_global(chaiscript::const_var(GLFW_KEY_Z),"KEY_Z");
      }

      void ChaiScriptInstance::initSceneObjectAPI() {
        std::cout << "ChaiScriptInstance: Init SceneObject API" << std::endl;
        mScript->add(chaiscript::user_type<SceneObject>(), "SceneObject");
        mScript->add_global(chaiscript::var(mParent),"gSceneObject");
        mScript->add(chaiscript::fun(&SceneObject::getAnimationAssetInstance),"getAnimationAsset");
        mScript->add(chaiscript::fun(&SceneObject::getAudioAssetInstance),"getAudioAsset");
        mScript->add(chaiscript::fun(&SceneObject::getLightAssetInstance),"getLightAsset");
        mScript->add(chaiscript::fun(&SceneObject::getModelAssetInstance),"getModelAsset");
        mScript->add(chaiscript::fun(&SceneObject::getShaderAssetInstance),"getShaderAsset");
        mScript->add(chaiscript::fun(&SceneObject::getScriptAssetInstance),"getScriptAsset");
        mScript->add(chaiscript::fun(&SceneObject::getPhysicsObjectAssetInstance),"getPhysicsObjectAsset");
        mScript->add(chaiscript::bootstrap::standard_library::vector_type<std::vector<float>>("FloatVector"));
        mScript->add(chaiscript::fun(&SceneObject::getTranslation),"getTranslation");
        mScript->add(chaiscript::fun<void,SceneObject,float,float,float>(&SceneObject::setTranslation),"setTranslation");
        mScript->add(chaiscript::fun<void,SceneObject,std::vector<float>>(&SceneObject::setTranslation),"setTranslation");
        mScript->add(chaiscript::fun(&SceneObject::getRotation),"getRotation");
        mScript->add(chaiscript::fun<void, SceneObject, float, float, float>(&SceneObject::setRotation),"setRotation");
        mScript->add(chaiscript::fun<void,SceneObject,std::vector<float>>(&SceneObject::setRotation),"setRotation");
        mScript->add(chaiscript::fun(&SceneObject::getScale),"getScale");
        mScript->add(chaiscript::fun<void,SceneObject,float,float,float>(&SceneObject::setScale),"setScale");
        mScript->add(chaiscript::fun<void,SceneObject,std::vector<float>>(&SceneObject::setScale),"setScale");
      }

      void ChaiScriptInstance::update() {
        auto updateFunction = mScript->eval<std::function<void(void)>>("update");
        updateFunction();
      }

      void ChaiScriptInstance::processInputs(std::vector<Components::Input::InputEvent> inputEventVector) {
          std::cout << "ChaiScriptInstance: Processing Input Events" << std::endl;
          for(std::vector<Components::Input::InputEvent>::iterator it = inputEventVector.begin();
              it != inputEventVector.end();
              it++) {
            auto processFunction = mScript->eval<std::function<void(Components::Input::InputEvent)>>("onInputEvent");
            processFunction(*it);
            Components::Input::InputEvent::LastEvent = (*it);
          }
      }

      void ChaiScriptInstance::initVideoComponentAPI() {
        mScript->add(chaiscript::user_type<Components::Video::VideoComponentInterface>(),"VideoComponent");
        mScript->add(chaiscript::fun(&Components::Video::VideoComponentInterface::closeWindow),"closeWindow");
        mScript->add(chaiscript::fun(&Components::Video::VideoComponentInterface::setCursorEnabled),"setCursorEnabled");
      }

      void ChaiScriptInstance::initAnimationInstanceAPI() {
        mScript->add(chaiscript::user_type<Components::Animation::AnimationInstance>(),"AnimationInstance");
        mScript->add(chaiscript::fun(&Components::Animation::AnimationInstance::isLooping),"isLooping");
        mScript->add(chaiscript::fun(&Components::Animation::AnimationInstance::setLooping),"setLooping");
      }

      void ChaiScriptInstance::initAnimationComponentAPI() {
        mScript->add(chaiscript::user_type<Components::Animation::AnimationComponentInterface>(),"AnimationComponent");
        mScript->add(chaiscript::fun(&Components::Animation::AnimationComponentInterface::play),"play");
        mScript->add(chaiscript::fun(&Components::Animation::AnimationComponentInterface::pause),"pause");
        mScript->add(chaiscript::fun(&Components::Animation::AnimationComponentInterface::stop),"stop");
        mScript->add(chaiscript::fun(&Components::Animation::AnimationComponentInterface::isLooping),"isLooping");
        mScript->add(chaiscript::fun(&Components::Animation::AnimationComponentInterface::setLooping),"setLooping");
      }

      void ChaiScriptInstance::initSceneAPI() {
        mScript->add(chaiscript::user_type<Scene>(),"Scene");
        mScript->add_global(chaiscript::var(Dream::getProject()->getActiveScene()),"gScene");
        mScript->add(chaiscript::fun(&getSceneObjectByUUID),"getSceneObjectByUUID");
      }
    } // End of Script
  } // End of Components
} // End of Dream