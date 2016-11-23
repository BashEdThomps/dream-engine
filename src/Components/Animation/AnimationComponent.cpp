/*
* AnimationComponent
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


#include "AnimationComponent.h"

namespace Dream {
  namespace Components {
    namespace Animation {

      AnimationComponent::AnimationComponent(Time* time) : Dream::Components::ComponentInterface() {
        mTime = time;
      }

      AnimationComponent::~AnimationComponent() {

      }

      bool AnimationComponent::init() {
        std::cout << "AnimationComponent: Provisional init returning true" << std::endl;
        return true;
      }

      void AnimationComponent::update(Scene *scene) {
        std::vector<SceneObject*> scenegraph = scene->getScenegraphVector();
        std::vector<SceneObject*>::iterator sgIter;
        for (sgIter = scenegraph.begin(); sgIter != scenegraph.end(); sgIter++) {
          SceneObject* currentSceneObject = *sgIter;
          AssetInstance* animAsset = currentSceneObject->getAnimationAssetInstance();
          if (animAsset != NULL) {
            try {
              AnimationInstance* dreamAnimInstance;
              dreamAnimInstance = dynamic_cast<AnimationInstance*>(animAsset);
              dreamAnimInstance->step(mTime->getTimeDelta());
              dreamAnimInstance->applyTransform(currentSceneObject);
            } catch (const std::exception &ex) {
              std::cerr << "AnimationComponent: Unable to cast animation asset to Dream Animation" << std::endl;
              std::cerr << ex.what() << std::endl;
            }
          }
        }
      }

      void AnimationComponent::play(AssetInstance *asset) {
        try {
          Components::Animation::AnimationInstance* animAsset;
          animAsset = dynamic_cast<Components::Animation::AnimationInstance*>(asset);
          animAsset->play();
        } catch (std::exception & ex) {
          std::cerr << "AnimationComponent: Exception playing animation." << ex.what() << std::endl;
        }
      }

      void AnimationComponent::pause(AssetInstance *asset) {
        try {
          Components::Animation::AnimationInstance* animAsset;
          animAsset = dynamic_cast<Components::Animation::AnimationInstance*>(asset);
          animAsset->pause();
        } catch (std::exception & ex) {
          std::cerr << "AnimationComponent: Exception pausing animation." << ex.what() << std::endl;
        }
      }

      void AnimationComponent::stop(AssetInstance *asset) {
        try {
          Components::Animation::AnimationInstance* animAsset;
          animAsset = dynamic_cast<Components::Animation::AnimationInstance*>(asset);
          animAsset->stop();
        } catch (std::exception & ex) {
          std::cerr << "AnimationComponent: Exception stopping animation." << ex.what() << std::endl;
        }
      }

      bool AnimationComponent::isLooping(AssetInstance* asset) {
        if (!asset) {
          std::cerr << "AnimationComponent: asset is null in isLooping... " << std::endl;
          return false;
        }

        try {
          Components::Animation::AnimationInstance* animationAsset;
          animationAsset = dynamic_cast<Components::Animation::AnimationInstance*>(asset);
          if (!animationAsset) {
            std::cerr << "AnimationComponent: animationAsset is null in isLooping... " << std::endl;
            return false;
          }
          return animationAsset->isLooping();
        } catch (std::exception &ex) {
          std::cerr << "AnimationComponent: Exception in isLooping... " << std::endl;
          std::cerr << ex.what() << std::endl;
        }
        return false;
      }

      void AnimationComponent::setLooping(AssetInstance* asset, bool looping) {
        try {
          Components::Animation::AnimationInstance* animationAsset;
          animationAsset = dynamic_cast<Components::Animation::AnimationInstance*>(asset);
          animationAsset->setLooping(looping);
          std::cout << "AnimationComponent: Setting " << animationAsset->getNameAndUUIDString()
                    << " looping: " << String::boolToYesNo(looping) << std::endl;
        } catch (std::exception &ex) {
          std::cerr << "AnimationComponent: Exception in setLooping... " << std::endl;
          std::cerr << ex.what() << std::endl;
        }
      }

    } // End of Animation
  } // End of Components
} // End of Dream
