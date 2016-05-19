/*
* VideoInterface
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

#include "VideoInterface.h"

namespace Dream {
	namespace Plugins {
		namespace Video {
            VideoInterface::VideoInterface(void) : Dream::Plugins::PluginInterface() {
                setScreenWidth(VIDEO_INTERFACE_DEFAULT_SCREEN_WIDTH);
                setScreenHeight(VIDEO_INTERFACE_DEFAULT_SCREEN_HEIGHT);
            }
            
            void VideoInterface::setScreenWidth(int width) {
                mScreenWidth = width;
            }
            
            int  VideoInterface::getScreenWidth() {
                return mScreenWidth;
            }
                
            void VideoInterface::setScreenHeight(int height) {
                mScreenHeight = height;
            }
            
            int  VideoInterface::getScreenHeight() {
                return mScreenHeight;
            }
                
            void VideoInterface::setScreenName(std::string name) {
                mScreenName = name;
            }
            
            std::string VideoInterface::getScreenName() {
                return mScreenName;
            }
                
		} // End of Video
	} // End of Plugins
} // End of Dream
