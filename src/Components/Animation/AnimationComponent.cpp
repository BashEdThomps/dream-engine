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

      AnimationComponent::AnimationComponent() : ComponentInterface() {}

      AnimationComponent::~AnimationComponent() {

        if (DEBUG) {
            cout << "AnimationComponent: Destroying Object" << endl;
        }

      }

      bool AnimationComponent::init() {
        if (DEBUG) {
            cout << "AnimationComponent: Initialisation Done." << endl;
        }
        return true;
      }

      void AnimationComponent::update(Scene *scene) {
        vector<SceneObject*> scenegraph = scene->getScenegraphVector();
        vector<SceneObject*>::iterator sgIter;
        for (sgIter = scenegraph.begin(); sgIter != scenegraph.end(); sgIter++) {
          SceneObject* currentSceneObject = *sgIter;
          AssetInstance* animAsset = currentSceneObject->getAnimationInstance();
          if (animAsset != nullptr) {
            try {
              AnimationInstance* dreamAnimInstance;
              dreamAnimInstance = dynamic_cast<AnimationInstance*>(animAsset);
              dreamAnimInstance->step(mTime->getTimeDelta());
              dreamAnimInstance->applyTransform(currentSceneObject->getTransform());
            } catch (const exception &ex) {
              cerr << "AnimationComponent: Unable to cast animation asset to Dream Animation" << endl;
              cerr << ex.what() << endl;
            }
          }
        }
      }

      void AnimationComponent::play(AssetInstance *asset) {
        try {
          AnimationInstance* animAsset;
          animAsset = dynamic_cast<AnimationInstance*>(asset);
          animAsset->play();
        } catch (exception & ex) {
          cerr << "AnimationComponent: Exception playing animation." << ex.what() << endl;
        }
      }

      void AnimationComponent::pause(AssetInstance *asset) {
        try {
          AnimationInstance* animAsset;
          animAsset = dynamic_cast<AnimationInstance*>(asset);
          animAsset->pause();
        } catch (exception & ex) {
          cerr << "AnimationComponent: Exception pausing animation." << ex.what() << endl;
        }
      }

      void AnimationComponent::stop(AssetInstance *asset) {
        try {
          AnimationInstance* animAsset;
          animAsset = dynamic_cast<AnimationInstance*>(asset);
          animAsset->stop();
        } catch (exception & ex) {
          cerr << "AnimationComponent: Exception stopping animation." << ex.what() << endl;
        }
      }

      bool AnimationComponent::isLooping(AssetInstance* asset) {
        if (!asset) {
          cerr << "AnimationComponent: asset is null in isLooping... " << endl;
          return false;
        }

        try {
          AnimationInstance* animationAsset;
          animationAsset = dynamic_cast<AnimationInstance*>(asset);
          if (!animationAsset) {
            cerr << "AnimationComponent: animationAsset is null in isLooping... " << endl;
            return false;
          }
          return animationAsset->isLooping();
        } catch (exception &ex) {
          cerr << "AnimationComponent: Exception in isLooping... " << endl;
          cerr << ex.what() << endl;
        }
        return false;
      }

      void AnimationComponent::setLooping(AssetInstance* asset, bool looping) {
        try {
          AnimationInstance* animationAsset;
          animationAsset = dynamic_cast<AnimationInstance*>(asset);
          animationAsset->setLooping(looping);
          if (DEBUG) {
            cout << "AnimationComponent: Setting " << animationAsset->getNameAndUuidString()
                 << " looping: " << String::boolToYesNo(looping) << endl;
          }
        } catch (exception &ex) {
          cerr << "AnimationComponent: Exception in setLooping... " << endl;
          cerr << ex.what() << endl;
        }
      }

} // End of Dream
