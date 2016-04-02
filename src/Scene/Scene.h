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

#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include "Scenegraph.h"
#include "Camera.h"

namespace Dream {
	namespace Scene {
		class Scene {
		private:
			std::string mName;
			Scenegraph *mScenegraph;
			Camera* mCamera;
			bool mCollisionEnabled;
			bool mPhysicsEnabled;
			bool mAnimationEnabled;
			bool mInputEnabled;
			bool mAudioEnabled;
		public:
			Scene();
			~Scene();
			std::string getName();
			void        setName(std::string);
			Scenegraph* getScenegraph();
		//protected:
		//private:
		}; // End of Scene
	} // End of Scene
} // End of Dream

#endif // End of SCENE_H
