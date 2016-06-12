/*
* Dream::Scene::Scene
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

#include "Scene.h"

namespace Dream {
namespace  Scene {
	Scene::Scene() {
		mPhysicsEnabled   = false;
		mAnimationEnabled = false;
		mInputEnabled     = false;
		mAudioEnabled     = false;
    mScriptingEnabled = false;
	}

	Scene::Scene(nlohmann::json jsonScene) {
		mUUID = jsonScene[SCENE_JSON_UUID];
		mName = jsonScene[SCENE_JSON_NAME];

		mPhysicsEnabled = (
			jsonScene[SCENE_JSON_PHYSICS_ENABLED].is_null() ?
			false : (bool)jsonScene[SCENE_JSON_PHYSICS_ENABLED]
		);

		mAnimationEnabled = (
			jsonScene[SCENE_JSON_ANIMATION_ENABLED].is_null() ?
			false : (bool)jsonScene[SCENE_JSON_ANIMATION_ENABLED]
		);

		mInputEnabled = (
			jsonScene[SCENE_JSON_INPUT_ENABLED].is_null() ?
			false :(bool)jsonScene[SCENE_JSON_INPUT_ENABLED]
		);

		mAudioEnabled = (
			jsonScene[SCENE_JSON_AUDIO_ENABLED].is_null() ?
			false : (bool)jsonScene[SCENE_JSON_AUDIO_ENABLED]
		);
            
		mScriptingEnabled = (
			jsonScene[SCENE_JSON_SCRIPTING_ENABLED].is_null() ?
      false : (bool)jsonScene[SCENE_JSON_SCRIPTING_ENABLED]
    );
		
		loadDefaultCameraTransform(jsonScene[SCENE_JSON_CAMERA]);
		
		nlohmann::json sceneObjects = jsonScene[SCENE_JSON_SCENE_OBJECTS];

		if (!sceneObjects.is_null() && sceneObjects.is_array()) {
			loadSceneObjects(sceneObjects,NULL);
		}
	}
	
	void Scene::loadDefaultCameraTransform(nlohmann::json camera) {
		mDefaultCameraTranslation = std::vector<float>(3);
		mDefaultCameraRotation = std::vector<float>(3);
		
		if (!camera.is_null()) {
			nlohmann::json translation   = camera[SCENE_JSON_TRANSLATION];
			setDefaultCameraTranslation(
			  translation[SCENE_JSON_X],
				translation[SCENE_JSON_Y],
				translation[SCENE_JSON_Z]
			);
			
			nlohmann::json rotation   = camera[SCENE_JSON_ROTATION];
			setDefaultCameraRotation(
			  rotation[SCENE_JSON_X],
			  rotation[SCENE_JSON_Y],
			  rotation[SCENE_JSON_Z]
			);
		} else {
			setDefaultCameraTranslation (0.0f, 0.0f, 0.0f);
			setDefaultCameraRotation    (0.0f, 0.0f, 0.0f);
		}
	}
	
	void Scene::setDefaultCameraTranslation(float x, float y, float z) {
		mDefaultCameraTranslation[0] = x;
		mDefaultCameraTranslation[1] = y;
		mDefaultCameraTranslation[2] = z;
	}
	
	void Scene::setDefaultCameraRotation(float x, float y, float z) {
		mDefaultCameraRotation[0] = x;
		mDefaultCameraRotation[1] = y;
		mDefaultCameraRotation[2] = z;
	}

	Scene::~Scene() {}

	bool Scene::init() {
		std::cout << "Scene: Initialising Scene " << getName() << "(" << getUUID() << ")" << std::endl;
		return true;
	}
	
	std::vector<float> Scene::getDefaultCameraTranslation() {
		return mDefaultCameraTranslation;
	}
	
	std::vector<float> Scene::getDefaultCameraRotation() {
		return mDefaultCameraRotation;
	}
	
	bool Scene::isScenegraphVectorEmpty() {
		return mScenegraphVector.empty();
	}
	
	std::string Scene::getName() {
		return mName;
	}

	std::string Scene::getUUID() {
		return mUUID;
	}

	void Scene::setUUID(std::string uuid) {
		mUUID = uuid;
	}

	void Scene::setName(std::string name) {
		mName = name;
	}

	bool Scene::isAudioEnabled() {
		return mAudioEnabled;
	}

	bool Scene::isAnimationEnabled() {
		return mAnimationEnabled;
	}

	bool Scene::isPhysicsEnabled() {
		return mPhysicsEnabled;
	}

	bool Scene::isInputEnabled() {
		return mInputEnabled;
	}
	
	void Scene::loadSceneObjects(nlohmann::json jsonArray, SceneObject* parent) {
		//std::cout << "Loading scene objects from array: "<< jsonArray.dump() << std::endl;
		if (!jsonArray.is_null()) {
			for (nlohmann::json::iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
				//std::cout << "Scene: Creating SceneObject " << std::endl;
				SceneObject *nextSceneObject = new SceneObject(*it);
				if (parent != NULL) {
					nextSceneObject->setParent(parent);
					parent->addChild(nextSceneObject);
				} else {
					setRootSceneObject(nextSceneObject);
				}
				if (!((*it)[SCENE_OBJECT_CHILDREN]).is_null()){
					loadSceneObjects((*it)[SCENE_OBJECT_CHILDREN],nextSceneObject);
				}
				
				nextSceneObject->showStatus();
			}
		}
	}

	bool Scene::hasSceneObect(SceneObject *obj) {
		return mRootSceneObject == obj || mRootSceneObject->isParentOfDeep(obj);
	}

	std::string Scene::generateSceneObjectPath(SceneObject* so) {
		std::stringstream stream;
		std::vector<std::string> pathVector;

		SceneObject* next = so;
		while (next != NULL) {
			pathVector.push_back(next->getUUID());
			next = next->getParent();
		}

		std::reverse(pathVector.begin(),pathVector.end());
		for (std::vector<std::string>::iterator it = pathVector.begin(); it != pathVector.end(); ++it) {
			stream << PATH_DELIMETER << *it;
		}
		std::string retval = stream.str();
		return retval;
	}

	SceneObject* Scene::getSceneObjectByUUID(std::string uuid) {
		for (std::vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++) {
		  if ((*it)->hasUUID(uuid)){
				return (*it);
			}
		}
		return NULL;
	}

	int Scene::getNumberOfSceneObjects() {
		return mScenegraphVector.size();
	}

	void Scene::showStatus() {
		std::cout << "Scene:" << std::endl;
		std::cout << "             UUID: " << mUUID << std::endl;
		std::cout << "             Name: " << mName << std::endl;
		std::cout << "    Audio Enabled: " << Util::StringUtils::boolToYesNo(isAudioEnabled()) << std::endl;
		std::cout << "Animation Enabled: " << Util::StringUtils::boolToYesNo(isAnimationEnabled()) << std::endl;
		std::cout << "    Input Enabled: " << Util::StringUtils::boolToYesNo(isInputEnabled()) << std::endl;
		std::cout << "  Physics Enabled: " << Util::StringUtils::boolToYesNo(isPhysicsEnabled()) << std::endl;
		std::cout << " Camera Transform: " << std::endl;
		
		std::cout << "      Translation: " << "("
		          << mDefaultCameraTranslation[0] << ","
		          << mDefaultCameraTranslation[1] << ","
		          << mDefaultCameraTranslation[2] << ")" << std::endl;
		
		std::cout << "         Rotation: "     << "("
		          << mDefaultCameraRotation[0] << ","
		          << mDefaultCameraRotation[1] << ","
		          << mDefaultCameraRotation[2] << ")" << std::endl;
		
		std::cout << "    Scene Objects: " << getNumberOfSceneObjects() << std::endl;
		showScenegraph();
	}

	void Scene::showScenegraph() {
		if (mRootSceneObject == NULL) {
			std::cout << "Scenegraph is empty!" << std::endl;
			return;
		}
		generateScenegraphVector();
		generateSceneObjectPaths();
		for(std::vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++) {
			std::cout << (*it)->getNameUUIDString() << " @ " << (*it)->getPath() << std::endl;
		}
	}

	void Scene::setRootSceneObject(SceneObject* root) {
		mRootSceneObject = root;
	}

	SceneObject* Scene::getRootSceneObject() {
		return mRootSceneObject;
	}

	void Scene::generateScenegraphVector() {
		std::cout << "Scene: Regenerating Scene Graph Vector" << std::endl;
		mScenegraphVector.clear();
		mRootSceneObject->getChildrenVectorDeep(&mScenegraphVector);
	}

	void Scene::generateSceneObjectPaths() {
		for(std::vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++) {
			 (*it)->generatePath();
		}
	}
	
	std::vector<SceneObject*> Scene::getScenegraphVector() {
		return mScenegraphVector;
	}
	
}// End of Scene
} // End of Dream
