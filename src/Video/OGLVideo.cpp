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

#include "OGLVideo.h"

namespace Dream {
	namespace Video {
		OGLVideo::OGLVideo(void) : VideoInterface() {
		}

		OGLVideo::~OGLVideo(void) {
	    glfwTerminate();
		}

		bool OGLVideo::init(void) {
			std::cout << "OGLVideo: Initialising...";
			/* Initialize the library */
	    if (!glfwInit()) {
				return false;
			}
	    /* Create a windowed mode window and its OpenGL context */
	    mWindow = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	    if (!mWindow) {
	        glfwTerminate();
	        return false;
	    }
	    /* Make the window's context current */
	    glfwMakeContextCurrent(mWindow);
	    /* Loop until the user closes the window */
			std::cout << "done." << std::endl;
			return true;
		}

		void OGLVideo::update(void) {
	    if (!glfwWindowShouldClose(mWindow)) {

	        /*
					 * Render here
					 */

	        /* Swap front and back buffers */
	        glfwSwapBuffers(mWindow);
	        /* Poll for and process events */
	        glfwPollEvents();
	    }
		}
	} // End of Video
} // End of Dream
