/*
* Scene
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
			mCamera = new Camera();
			mScenegraph = new Scenegraph();
			mCollisionEnabled = false;;
			mPhysicsEnabled   = false;
			mAnimationEnabled = false;
			mInputEnabled     = false;
			mAudioEnabled     = false;
		}

		Scene::~Scene() {
		}

		std::string Scene::getName() {
			return mName;
		}

		void Scene::setName(std::string name) {
			mName = name;
		}

		Scenegraph* Scene::getScenegraph() {
			return mScenegraph;
		}

	}// End of Scene
} // End of Dream