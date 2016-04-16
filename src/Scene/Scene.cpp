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
			init();
			mPhysicsEnabled   = false;
			mAnimationEnabled = false;
			mInputEnabled     = false;
			mAudioEnabled     = false;
			mVideoEnabled     = false;
		}

		Scene::Scene(nlohmann::json jsonScene) {
			init();
			mName             = jsonScene [SCENE_JSON_NAME];
			mPhysicsEnabled   = jsonScene [SCENE_JSON_PHYSICS_ENABLED];
			mAnimationEnabled = jsonScene [SCENE_JSON_ANIMATION_ENABLED];
			mInputEnabled     = jsonScene [SCENE_JSON_INPUT_ENABLED];
			mAudioEnabled     = jsonScene [SCENE_JSON_AUDIO_ENABLED];
			mVideoEnabled     = jsonScene [SCENE_JSON_VIDEO_ENABLED];
		}

		Scene::~Scene() {

		}

		void Scene::init() {
			mCamera = new Camera();
			mScenegraph = new Scenegraph();
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

		Scenegraph* Scene::getScenegraph() {
			return mScenegraph;
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

		bool Scene::isCollisionEnabled() {
			return mCollisionEnabled;
		}

		bool Scene::isInputEnabled() {
			return mInputEnabled;
		}

		bool Scene::initBullet() {
			mBroadphase = new btDbvtBroadphase();
			mCollisionConfiguration = new btDefaultCollisionConfiguration();
			mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
			mSolver = new btSequentialImpulseConstraintSolver();
			mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mBroadphase,mSolver,mCollisionConfiguration);
		}

		bool Scene::setGravity3f(float x, float y, float z) {
			setGravityBtVector(new btVector3(x,y,z));
		}

		bool Scene::setGravityBtVector3(Gravity) {
			mDynamicsWorld->setGravity(gravity);
		}

	}// End of Scene
} // End of Dream
