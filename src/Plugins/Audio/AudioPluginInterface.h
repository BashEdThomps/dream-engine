/*
* AudioInterface
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

#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#include "../PluginInterface.h"

namespace Dream   {
namespace Plugins {
namespace Audio   {
	
	class AudioPluginInterface : public Dream::Plugins::PluginInterface {
	public:
		AudioPluginInterface(void);
		virtual ~AudioPluginInterface(void) {};
	}; // End of AudioPluginInterface
	
} // End of Plugins
} // End of Audio
} // End of Dream

#endif // End of AUDIOINTERFACE_H
