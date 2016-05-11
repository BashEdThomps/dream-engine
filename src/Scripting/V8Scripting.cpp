/*
* Dream::Scripting::V8Scripting
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

#include "V8Scripting.h"

namespace Dream {
	namespace Scripting {
		V8Scripting::V8Scripting(void) : ScriptingInterface() {

		}

		V8Scripting::~V8Scripting(void) {
			mContext.Reset();
			mIsolate->Dispose();
			v8::V8::Dispose();
			v8::V8::ShutdownPlatform();
			//delete mPlatform;
		}

		bool V8Scripting::init()  {
			// Initialize V8.
			v8::V8::InitializeICU();
			//V8::InitializeExternalStartupData(argv[0]);
			mPlatform = v8::platform::CreateDefaultPlatform();
			v8::V8::InitializePlatform(mPlatform);
			v8::V8::Initialize();
			mIsolate = v8::Isolate::GetCurrent();
			v8::Isolate::Scope isolate_scope(mIsolate);
			mContext.Reset(mIsolate, v8::Context::New(mIsolate));
			return true;
		}

		void V8Scripting::update() {

		}
	} // End of Scripting
} // End of Dream
