/*
* Dream::Components::Animation::DreamAnimation
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


#include "DreamAnimation.h"
#include "../../../Project/Project.h"

namespace Dream      {
namespace Components {
namespace Animation  {
namespace Dream      {
	
	DreamAnimation::DreamAnimation() : AnimationComponentInterface() {}
	DreamAnimation::~DreamAnimation() {}
	
	bool DreamAnimation::init() {
		std::cout << "DreamAnimation: Provisional init returning true" << std::endl;
		return true;
	}
	
	void DreamAnimation::update(Scene::Scene *scene) {
		std::vector<Scene::SceneObject*> scenegraph = scene->getScenegraphVector();
		std::vector<Scene::SceneObject*>::iterator sgIter;
		for (sgIter = scenegraph.begin(); sgIter != scenegraph.end(); sgIter++) {
			Scene::SceneObject* currentSceneObject = *sgIter;
			Asset::AssetInstance* animAsset = currentSceneObject->getAnimationAssetInstance();
			if (animAsset != NULL) {
				try {
					Asset::Instances::Animation::Dream::AnimationInstance* dreamAnimInstance;
					dreamAnimInstance = dynamic_cast<Asset::Instances::Animation::Dream::AnimationInstance*>(animAsset);
					dreamAnimInstance->step(Project::Project::getTime()->getTimeDelta());
					dreamAnimInstance->applyTransform(currentSceneObject);
				} catch (const std::exception &ex) {
					std::cerr << "DreamAnimation: Unable to cast animation asset to Dream Animation" << std::endl;
					std::cerr << ex.what() << std::endl;
				}
			}
		}
	}
	
	void DreamAnimation::play(Asset::AssetInstance *asset) {
		try {
			Asset::Instances::Animation::Dream::AnimationInstance* animAsset;
			animAsset = dynamic_cast<Asset::Instances::Animation::Dream::AnimationInstance*>(asset);
			animAsset->play();
		} catch (std::exception & ex) {
			std::cerr << "DreamAnimation: Exception playing animation." << ex.what() << std::endl;
		}
	}
	
	void DreamAnimation::pause(Asset::AssetInstance *asset) {
		try {
			Asset::Instances::Animation::Dream::AnimationInstance* animAsset;
			animAsset = dynamic_cast<Asset::Instances::Animation::Dream::AnimationInstance*>(asset);
			animAsset->pause();
		} catch (std::exception & ex) {
			std::cerr << "DreamAnimation: Exception pausing animation." << ex.what() << std::endl;
		}
	}
	
	void DreamAnimation::stop(Asset::AssetInstance *asset) {
		try {
			Asset::Instances::Animation::Dream::AnimationInstance* animAsset;
			animAsset = dynamic_cast<Asset::Instances::Animation::Dream::AnimationInstance*>(asset);
			animAsset->stop();
		} catch (std::exception & ex) {
			std::cerr << "DreamAnimation: Exception stopping animation." << ex.what() << std::endl;
		}
	}
	
	bool DreamAnimation::isLooping(Asset::AssetInstance* asset) {
		try {
			Asset::Instances::Animation::Dream::AnimationInstance* animationAsset;
			animationAsset = dynamic_cast<Asset::Instances::Animation::Dream::AnimationInstance*>(asset);
			return animationAsset->isLooping();
		} catch (std::exception &ex) {
			std::cerr << "DreamAnimation: Exception in isLooping... " << std::endl;
			std::cerr << ex.what() << std::endl;
		}
		return false;
	}
	
	void DreamAnimation::setLooping(Asset::AssetInstance* asset, bool looping) {
		try {
			Asset::Instances::Animation::Dream::AnimationInstance* animationAsset;
			animationAsset = dynamic_cast<Asset::Instances::Animation::Dream::AnimationInstance*>(asset);
			animationAsset->setLooping(looping);
			std::cout << "DreamAnimation: Setting " << animationAsset->getNameAndUUIDString()
			          << " looping: " << Util::String::boolToYesNo(looping) << std::endl;
		} catch (std::exception &ex) {
			std::cerr << "DreamAnimation: Exception in setLooping... " << std::endl;
			std::cerr << ex.what() << std::endl;
		}
	}
	
} // End of Dream
} // End of Animation
} // End of Components
} // End of Dream

