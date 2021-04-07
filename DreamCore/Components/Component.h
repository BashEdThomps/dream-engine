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

#include <memory>
#include <optional>

using std::optional;
using std::reference_wrapper;

namespace octronic::dream
{
    class SceneRuntime;
    class EntityRuntime;
    class ProjectRuntime;
    /**
     * @brief Base class for Component classes.
     */
    class Component
    {
    public:
        Component();
        Component(ProjectRuntime& pr);

        Component(Component&&) = default;
        Component& operator=(Component&&) = default;

        Component(const Component&) = delete;
        Component& operator=(const Component&) = delete;

        virtual bool init() = 0;
        virtual void pushTasks() = 0;
        void setProjectRuntime(ProjectRuntime& pr);
        ProjectRuntime& getProjectRuntime();
    protected:
         optional<reference_wrapper<ProjectRuntime>> mProjectRuntime;
    };
}
