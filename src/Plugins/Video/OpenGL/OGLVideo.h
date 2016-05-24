/*
* OGLVideo
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

#ifndef OGLVIDEO_H
#define OGLVIDEO_H

#include "Camera.h"

#include "../VideoPluginInterface.h"

#include "../../../Asset/Instances/Model/WaveFront/ObjModelInstance.h"
#include "../../../Asset/Instances/Shader/ShaderInstance.h"

#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Dream {
namespace Plugins {
namespace Video {
namespace OpenGL {
	
	class OGLVideo : public VideoPluginInterface {
	private:
		GLFWwindow *mWindow;
		Camera     *mCamera;
		float       mScreenRatio;
	public:
		OGLVideo(void);
		~OGLVideo(void);
		bool init(void);
		void setupView(Dream::Scene::Scene*);
		void update(Dream::Scene::Scene*);
		void drawObjModel(
			Dream::Asset::Instances::Model::WaveFront::ObjModelInstance*,
			Dream::Asset::Instances::Shader::ShaderInstance*
		);
		
		bool checkGLError(int);
		
	}; // End of OGLVideo
	
} // End of OpenGL
} // End of Video
} // End of Plugins
} // End of Dream

#endif // End of OGLVIDEO_H
