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

#include "Components/SharedAssetRuntime.h"
#include "Components/Event.h"

namespace octronic::dream
{
    class ScriptDefinition;
    class InputComponent;
    class EntityRuntime;
    class SceneRuntime;

    class ScriptRuntime : public SharedAssetRuntime
    {
    public:
        ScriptRuntime(ProjectRuntime&,ScriptDefinition&);
        ScriptRuntime(ScriptRuntime&&) = default;
        ScriptRuntime& operator=(ScriptRuntime&&) = default;

        bool loadFromDefinition() override;

        bool createEntityState(EntityRuntime& rt);
        bool removeEntityState(UuidType uuid);

        string getSource() const;
        void setSource(const string& source);

        bool executeOnInit(EntityRuntime& state);
        bool executeOnUpdate(EntityRuntime& state);
        bool executeOnEvent(EntityRuntime& state);

        bool executeOnInput(SceneRuntime&);

        bool registerInputScript();
        bool removeInputScript();

        void pushTasks() override;

        bool hasSource() const;

    private:
        string mSource;
    };
}
