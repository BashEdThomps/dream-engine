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
#include "../ScriptInstance.h"

namespace Dream
{
    class ScriptDefinition;
    class SceneObjectRuntime;

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
    private:
        bool mError;
    }; // End of JSScriptInstance
} // End of Dream
