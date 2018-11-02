/*
* LuaScriptInstance
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

#include "../IAssetInstance.h"
#include "../Event.h"

namespace Dream
{
    class ScriptDefinition;

    class LuaInstancePair
    {
    public:
        SceneObjectRuntime* runtime = nullptr;
        bool initialised = false;
        bool error = false;
    };

    class LuaScriptInstance : public IAssetInstance
    {
    public:
        LuaScriptInstance(ScriptDefinition*,SceneObjectRuntime*);
        ~LuaScriptInstance() override;
        bool load(string) override;
        void update();
        void loadExtraAttributes(nlohmann::json) override;
        void addInstance(SceneObjectRuntime*);
        void removeInstance(SceneObjectRuntime*);
        vector<SceneObjectRuntime*> getInstanceVector();
        string getSource() const;
        void setSource(string source);

        bool executeOnInit();
        bool executeOnUpdate();
        bool executeOnInput();
        bool executeOnEvent();
        bool executeOnNanoVG();

    private:
        vector<LuaInstancePair> mInstances;
        string mSource;
        bool mError;
    }; // End of LuaScriptInstance
} // End of Dream
