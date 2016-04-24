/*
* V8Scripting
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

#ifndef V8SCRIPTING_H
#define V8SCRIPTING_H

#include "ScriptingInterface.h"
#include "v8/libplatform.h"
#include "v8/v8.h"

//using namespace v8;

namespace Dream {
	namespace Scripting {
		class V8Scripting : public ScriptingInterface {
		private:
			v8::Platform *mPlatform;
			v8::Isolate  *mIsolate;
			v8::Persistent<v8::Context> mContext;
		public:
			V8Scripting(void);
			~V8Scripting(void);
		}; // End of V8Scripting
	} // End of Scripting
} // End of Dream
#endif // End of V8SCRIPTING_H
