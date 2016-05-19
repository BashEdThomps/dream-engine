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

#ifndef VIDEOINTERFACE_H
#define VIDEOINTERFACE_H

#include <iostream>
#include "../PluginInterface.h"

#define VIDEO_INTERFACE_DEFAULT_SCREEN_WIDTH  1280
#define VIDEO_INTERFACE_DEFAULT_SCREEN_HEIGHT 720

namespace Dream {
	namespace Plugins {
		namespace Video {
			class VideoInterface : public Dream::Plugins::PluginInterface {
            protected:
                int mScreenWidth;
                int mScreenHeight;
                std::string mScreenName;
			public:
				VideoInterface(void);
				virtual ~VideoInterface(void) {};
                
                void setScreenWidth(int);
                int  getScreenWidth();
                
                void setScreenHeight(int);
                int  getScreenHeight();
                
                void        setScreenName(std::string);
                std::string getScreenName();
                
			}; // End of VideoInterface
		} // End of Video
	} // End of Plugins
} // End of Dream

#endif // End of VIDEOINTERFACE_H
