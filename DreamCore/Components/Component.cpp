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

#include "Component.h"
#include "Time.h"

using std::unique_lock;

namespace octronic::dream
{

    Component::Component
    (const string& className, ProjectRuntime* pr)
        : LockableObject(className),
          mEnabled(true),
          mProjectRuntime(pr)
    {

    }

    Component::~Component
    ()
    {

    }

    void
    Component::setTime
    (Time* time)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mTime = time;
    }

    bool Component::getEnabled() const
    {
        return mEnabled;
    }

    void Component::setEnabled(bool enabled)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mEnabled = enabled;
    }

    void Component::setProjectRuntime(ProjectRuntime *pr)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mProjectRuntime = pr;
    }
}
