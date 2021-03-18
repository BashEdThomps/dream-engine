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
        ScriptRuntime(const weak_ptr<ProjectRuntime>&,
                      const weak_ptr<ScriptDefinition>&);
        ~ScriptRuntime() override;
        bool loadFromDefinition() override;

        bool createEntityState(const weak_ptr<EntityRuntime>& rt);
        bool removeEntityState(UuidType uuid);

        string getSource() const;
        void setSource(const string& source);

        bool executeOnInit(const weak_ptr<EntityRuntime>& state);
        bool executeOnUpdate(const weak_ptr<EntityRuntime>& state);
        bool executeOnEvent(const weak_ptr<EntityRuntime>& state);

        bool executeOnInput(const weak_ptr<SceneRuntime>&);

        bool registerInputScript();
        bool removeInputScript();

        void pushTasks() override;

        bool hasSource() const;

    private:
        string mSource;
    };
}
