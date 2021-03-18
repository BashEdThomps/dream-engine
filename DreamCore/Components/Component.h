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

using std::weak_ptr;
using std::shared_ptr;
using std::enable_shared_from_this;

namespace octronic::dream
{
    class Time;
    class SceneRuntime;
    class EntityRuntime;
    class ProjectRuntime;

    /**
     * @brief Component is the base class for Component classes.
     * Providing access to octronic::dream::Time and octronic::dream::ProjectRuntime
     * and Enabled status flag;
     * Component publically inherits from octronic::dream::LockableObject, which
     * provide the means to build thread safety into the Component.
     */
    class Component : public enable_shared_from_this<Component>
    {

    public:
        Component(const weak_ptr<ProjectRuntime>& pr);
        virtual ~Component();
        virtual bool init() = 0;

        void setProjectRuntime(const weak_ptr<ProjectRuntime>& pr);

        virtual void pushTasks() = 0;

    protected:
         weak_ptr<ProjectRuntime> mProjectRuntime;
    };
}
