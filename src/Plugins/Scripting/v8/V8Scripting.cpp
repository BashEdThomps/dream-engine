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
namespace Plugins {
namespace Scripting {
namespace V8 {
	
	v8::Isolate::CreateParams _CreateParams;
	ArrayBufferAllocator      _ArrayBufferAllocator;

	V8Scripting::V8Scripting(void) : ScriptingPluginInterface() {}

	V8Scripting::~V8Scripting(void) {
		mContext.Reset();
		mIsolate->Dispose();
		v8::V8::Dispose();
		v8::V8::ShutdownPlatform();
	}

	bool V8Scripting::init()  {
		std::cout << "V8Scripting: Initialising...";
		v8::V8::InitializeICU();
		mPlatform = v8::platform::CreateDefaultPlatform();
		v8::V8::InitializePlatform(mPlatform);
		v8::V8::Initialize();
		mIsolate = v8::Isolate::GetCurrent();

		if(mIsolate == NULL) {
			_CreateParams.array_buffer_allocator = &_ArrayBufferAllocator;
			mIsolate = v8::Isolate::New(_CreateParams);
			mIsolate->Enter();
		}

		v8::Isolate::Scope isolate_scope(mIsolate);
		mContext.Reset(mIsolate, v8::Context::New(mIsolate));
		std::cout << "done." << std::endl;
		return true;
	}

	void V8Scripting::update(Dream::Scene::Scene* scene) {
	}
	
} // End of V8
} // End of Scripting
} // End of Plugins
} // End of Dream
