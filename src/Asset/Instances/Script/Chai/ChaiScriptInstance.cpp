/*
* Dream::Asset::Instances::Scripting::Chai::ChaiScriptInstance
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
#include "../../../../Main/Dream.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Script    {
namespace Chai      {
	
	ChaiScriptInstance::ChaiScriptInstance(AssetDefinition* definition) : AssetInstance(definition) {}
	ChaiScriptInstance::~ChaiScriptInstance(void) {}
	
	bool ChaiScriptInstance::load(std::string projectPath) {
		mProjectPath = projectPath;
		mScript = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());
		initChaiScriptAPI();
		chaiscript::Boxed_Value result = mScript->eval_file(projectPath+mDefinition->getAssetPath());
		initAPIs();
		return !result.is_null();
	}
	
	void ChaiScriptInstance::initAPIs() {
		initAssetInstanceAPI();
		initAssetManagerAPI();
		initAudioPluginAPI();
		initCameraAPI();
		initGlfwTimeAPI();
		initInputEventAPI();
		initKissFftAPI();
		initPluginManagerAPI();
		initProjectAPI();
		initSceneObjectAPI();
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
		scriptAsset = Dream::getProject()->getAssetDefinitionByUUID(uuid);
		std::cout << "ChaiAssetInstance: Importing ChaiScript Library: " << scriptAsset->getNameAndUUIDString() << std::endl;
		chaiscript::Boxed_Value result = mScript->eval_file(getProjectPath()+scriptAsset->getAssetPath());
		return !result.is_null();
	}

	void ChaiScriptInstance::initKissFftAPI() {
		mKissFFT = new FFT(NUM_FFT,false);
		//mScript->add(chaiscript::fun(&ChaiScriptInstance::transformFFT),"transformFFT");
		//mScript->add(chaiscript::fun(&ChaiScriptInstance::insertIntoFFTInputBuffer),"insertIntoFFTInputBuffer");
		//mScript->add(chaiscript::fun(&ChaiScriptInstance::getFFTOutputBuffer),"getFFTOutputBuffer");
		//mScript->add(chaiscript::fun(&ChaiScriptInstance::clearFFTBuffers),"clearFFTBuffers");
	}
	
	void ChaiScriptInstance::insertIntoFFTInputBuffer(cpx_type* data, int nSamples) {
		//mFFTInputBuffer.insert(mFFTInputBuffer.end(), data, data+nSamples);
	}
	
	std::vector<cpx_type> ChaiScriptInstance::getFFTOutputBuffer() {
		//return mFFTOutputBuffer;
	}
	
	void ChaiScriptInstance::clearFFTBuffers() {
		//mFFTInputBuffer.clear();
		//mFFTOutputBuffer.clear();
	}
	
	void ChaiScriptInstance::transformFFT() {
		//mKissFFT->transform(&mFFTInputBuffer[0], &mFFTOutputBuffer[0]);
	}
	
	void ChaiScriptInstance::initAssetInstanceAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init AssetInstance API" << std::endl;
		#endif
		mScript->add(chaiscript::user_type<AssetInstance>(),"AssetInstance");
	}
	
	void ChaiScriptInstance::initProjectAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init Project API" << std::endl;
		#endif
		mScript->add(chaiscript::user_type<Project::Project>(),"Project");
		mScript->add_global(chaiscript::var(Dream::getProject()),"gProject");
		mScript->add(chaiscript::fun(&Project::Project::getPluginManager),"getPluginManager");
		mScript->add(chaiscript::fun(&Project::Project::getAssetManager),"getAssetManager");
	}
	
	void ChaiScriptInstance::initAssetManagerAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init AssetManager API" << std::endl;
		#endif
		mScript->add(chaiscript::user_type<AssetManager>(),"AssetManager");
		mScript->add(chaiscript::fun(&AssetManager::getAssetInstanceByUUID),"getAssetInstanceByUUID");
	}

	void ChaiScriptInstance::initAudioPluginAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init AudioPlugin API" << std::endl;
		#endif
		mScript->add(chaiscript::user_type<Plugins::Audio::AudioPluginInterface>(),"AudioPluginInterface");
		mScript->add(chaiscript::fun(&Plugins::Audio::AudioPluginInterface::playAudioAsset),"playAudioAsset");
		mScript->add(chaiscript::fun(&Plugins::Audio::AudioPluginInterface::pauseAudioAsset),"pauseAudioAsset");
		mScript->add(chaiscript::fun(&Plugins::Audio::AudioPluginInterface::stopAudioAsset),"stopAudioAsset");
		mScript->add(
			chaiscript::fun<
				float,
				Plugins::Audio::AudioPluginInterface,
				Asset::AssetInstance*
			>
			(&Plugins::Audio::AudioPluginInterface::getSampleOffset),
			"getSampleOffset"
		);
	}
	
	void ChaiScriptInstance::initPluginManagerAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init PluginManager API" << std::endl;
		#endif
		mScript->add(chaiscript::user_type<Plugins::PluginManager>(),"PluginManager");
		mScript->add(chaiscript::fun(&Plugins::PluginManager::getAudioPlugin),"getAudioPlugin");
	}
	
	void ChaiScriptInstance::initGlfwTimeAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init GLFWTime API" << std::endl;
		#endif
		mScript->add(chaiscript::user_type<Plugins::Video::OpenGL::GLFWTime>(),"GLFWTime");
		mScript->add_global(chaiscript::var(Project::Project::getTime()),"gTime");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::GLFWTime::update),"update");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::GLFWTime::getCurrentTime),"getCurrentTime");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::GLFWTime::getLastTime),"getLastTime");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::GLFWTime::getTimeDelta),"getTimeDelta");
	}
	
	void ChaiScriptInstance::initCameraAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init Camera API" << std::endl;
		#endif
		// Class Definition
		mScript->add(chaiscript::user_type<Plugins::Video::OpenGL::Camera>(),"Camera");
		// Methods
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::Camera::processMouseScroll),"processMouseScroll");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::Camera::processKeyboard),"processKeyboard");
		mScript->add(chaiscript::fun(&Plugins::Video::OpenGL::Camera::processMouseMovement),"processMouseMovement");
		// Global Variables
		mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_FORWARD), "CAMERA_MOVEMENT_FORWARD");
		mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_BACKWARD),"CAMERA_MOVEMENT_BACKWARD");
		mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_LEFT),"CAMERA_MOVEMENT_LEFT");
		mScript->add_global(chaiscript::const_var(CAMERA_MOVEMENT_RIGHT),"CAMERA_MOVEMENT_RIGHT");
		// Camera Instance
		mScript->add_global(chaiscript::var(&Plugins::Video::OpenGL::OGLVideo::sCamera),"GLCamera");
		
	}
	
	void ChaiScriptInstance::initInputEventAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init Input Event API" << std::endl;
		#endif
		// Class Definition
		mScript->add(chaiscript::user_type<Plugins::Input::InputEvent>(),"InputEvent");
		// Methods
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getSource),"getSource");
		
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getXPosition),"getXPosition");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getYPosition),"getYPosition");
		
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getXPositionOffset),"getXPositionOffset");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getYPositionOffset),"getYPositionOffset");
		
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getXScrollOffset),"getXScrollOffset");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getYScrollOffset),"getYScrollOffset");
		
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getKey),"getKey");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getMods),"getMods");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getAction),"getAction");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getButton),"getButton");
		mScript->add(chaiscript::fun(&Plugins::Input::InputEvent::getScancode),"getScancode");
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
		mScript->add_global(chaiscript::const_var(GLFW_KEY_W),"KEY_W");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_A),"KEY_A");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_S),"KEY_S");
		mScript->add_global(chaiscript::const_var(GLFW_KEY_D),"KEY_D");
	}
	
	void ChaiScriptInstance::initSceneObjectAPI() {
		#ifdef VERBOSE
			std::cout << "ChaiScriptInstance: Init SceneObject API" << std::endl;
		#endif
		mScript->add(chaiscript::user_type<Scene::SceneObject>(),"SceneObject");
		mScript->add_global(chaiscript::var(mParent),"gSceneObject");
		mScript->add(chaiscript::fun(&Scene::SceneObject::getAnimationAssetInstance),"getAnimationAsset");
		mScript->add(chaiscript::fun(&Scene::SceneObject::getAudioAssetInstance),"getAudioAsset");
		mScript->add(chaiscript::fun(&Scene::SceneObject::getLightAssetInstance),"getLightAsset");
		mScript->add(chaiscript::fun(&Scene::SceneObject::getModelAssetInstance),"getModelAsset");
		mScript->add(chaiscript::fun(&Scene::SceneObject::getShaderAssetInstance),"getShaderAsset");
		mScript->add(chaiscript::fun(&Scene::SceneObject::getScriptAssetInstance),"getScriptAsset");
		mScript->add(chaiscript::fun(&Scene::SceneObject::getPhysicsObjectAssetInstance),"getPhysicsObjectAsset");
	}
	
	void ChaiScriptInstance::update() {
		auto updateFunction = mScript->eval<std::function<void(void)>>("update");
		updateFunction();
	}
	
	void ChaiScriptInstance::processInputs(std::vector<Plugins::Input::InputEvent> inputEventVector) {
    #ifdef VERBOSE
    	std::cout << "ChaiScriptInstance: Processing Input Events" << std::endl;
    #endif
		for(std::vector<Plugins::Input::InputEvent>::iterator it = inputEventVector.begin(); it != inputEventVector.end(); it++) {
			auto processFunction = mScript->eval<std::function<void(Plugins::Input::InputEvent)>>("onInputEvent");
			processFunction(*it);
			Plugins::Input::InputEvent::LastEvent = (*it);
		}
	}
	
} // End of ChaiScript
} // End of Script
} // End of Instance
} // End of Asset
} // End of Dream
