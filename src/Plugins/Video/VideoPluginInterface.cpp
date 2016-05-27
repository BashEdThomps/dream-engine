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

#include "VideoPluginInterface.h"

namespace Dream   {
namespace Plugins {
namespace Video   {
	
	VideoPluginInterface::VideoPluginInterface(void) : Dream::Plugins::PluginInterface() {
		setScreenWidth(VIDEO_INTERFACE_DEFAULT_SCREEN_WIDTH);
    setScreenHeight(VIDEO_INTERFACE_DEFAULT_SCREEN_HEIGHT);
  }
            
  void VideoPluginInterface::setScreenWidth(int width) {
		mScreenWidth = width;
  }
            
  int  VideoPluginInterface::getScreenWidth() {
		return mScreenWidth;
  }
                
  void VideoPluginInterface::setScreenHeight(int height) {
      mScreenHeight = height;
  }
  
  int  VideoPluginInterface::getScreenHeight() {
      return mScreenHeight;
  }
      
  void VideoPluginInterface::setScreenName(std::string name) {
      mScreenName = name;
  }
  
  std::string VideoPluginInterface::getScreenName() {
      return mScreenName;
  }
		
	bool VideoPluginInterface::isWindowShouldCloseFlagSet() {
		return mWindowShouldClose;
 	}

} // End of Video
} // End of Plugins
} // End of Dream
