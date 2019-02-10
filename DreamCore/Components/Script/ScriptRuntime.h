/*
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

#include "ScriptTasks.h"
#include "../SharedAssetRuntime.h"
#include "../Event.h"

class asIScriptModule;
class asIScriptFunction;
class asIScriptContext;

namespace Dream
{
    class ScriptDefinition;
    class InputComponent;
    class NanoVGComponent;
    class SceneRuntime;

    class ScriptRuntime : public SharedAssetRuntime
    {
    public:
        ScriptRuntime(ScriptDefinition*,ProjectRuntime*);
        ~ScriptRuntime() override;
        bool useDefinition() override;
        string getSource() const;
        void setSource(const string& source);

        bool executeOnInit(SceneObjectRuntime* state);
        bool executeOnUpdate(SceneObjectRuntime* state);
        bool executeOnEvent(SceneObjectRuntime* state);

        bool executeOnInput(InputComponent*, SceneRuntime*);
        bool executeOnNanoVG(NanoVGComponent*, SceneRuntime*);

        bool getInitialised() const;
    private:
        bool mError;
        bool mInitialised;
        string mSource;
        asIScriptModule* mScriptModule;
        asIScriptFunction *mInitFunction;
        asIScriptFunction *mUpdateFunction;
        asIScriptFunction *mEventFunction;
        asIScriptFunction *mNanoVGFunction;
        asIScriptFunction *mInputFunction;
        asIScriptContext *mContext;
        string mUuidString;
    };
}
