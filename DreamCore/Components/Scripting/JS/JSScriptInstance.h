/*
* JSScriptInstance
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

#pragma once

#include <vector>
#include <string>

#include "../../../Common/Constants.h"
#include "../../Event.h"
#include "../ScriptInstance.h"

#include <gainput/gainput.h>

#include <libplatform/libplatform.h>
#include <v8.h>

#include <v8pp/module.hpp>
#include <v8pp/class.hpp>


namespace Dream
{
    class ScriptDefinition;
    class SceneObjectRuntime;
    class ScriptCache;

    class JSScriptInstance : public ScriptInstance
    {

    public:
        JSScriptInstance(shared_ptr<ScriptDefinition>,shared_ptr<SceneObjectRuntime>);
        ~JSScriptInstance(void);
        bool load(string);
        void update();
        void loadExtraAttributes(json);
        bool getError();
        void setError(bool);
        bool inflate(shared_ptr<ScriptCache>);

        void onInit(shared_ptr<SceneObjectRuntime> so);
        void onEvent(shared_ptr<SceneObjectRuntime> so, Event ev);
        void onInput(shared_ptr<SceneObjectRuntime> so, gainput::InputMap* iMap);
        void onUpdate(shared_ptr<SceneObjectRuntime> so);
        void onNanoVG(shared_ptr<SceneObjectRuntime> so);

    private:
        bool mError;
        v8::Persistent<v8::Script> mScript;
        v8::Persistent<v8::Context> mContext;
        v8::Persistent<v8::Object> mPersistentSOR;

        // Functions we look for to call
        v8::Persistent<v8::Function> mOnInitFunction;
        v8::Persistent<v8::Function> mOnUpdateFunction;
        v8::Persistent<v8::Function> mOnEventFunction;
        v8::Persistent<v8::Function> mOnInputFunction;
        v8::Persistent<v8::Function> mOnNanoVGFunction;
    }; // End of JSScriptInstance
} // End of Dream
