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

#ifndef V8SCRIPTING_H
#define V8SCRIPTING_H

#include "../ScriptingPluginInterface.h"

#include <libplatform.h>
#include <v8.h>
#include <iostream>

namespace Dream {
namespace Plugins {
namespace Scripting {
namespace V8 {
	
	class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
	public:
		virtual void* Allocate(size_t length) {
			void* data = AllocateUninitialized(length);
			return data == NULL ? data : memset(data, 0, length);
		}
		
		virtual void* AllocateUninitialized(size_t length) {
			return malloc(length);
		}
		
		virtual void Free(void* data, size_t) {
			free(data);
		}
	}; // End of ArrayBuffer Allocator

	class V8Scripting : public ScriptingPluginInterface {
	private:
		v8::Platform *mPlatform;
		v8::Isolate  *mIsolate;
		v8::Persistent<v8::Context> mContext;
	public:
		V8Scripting(void);
		~V8Scripting(void);
		bool init(void);
		void update(Dream::Scene::Scene*);
	}; // End of V8Scripting
	
} // End of V8
} // End of Scripting
} // End of Plugins
} // End of Dream

#endif // End of V8SCRIPTING_H
