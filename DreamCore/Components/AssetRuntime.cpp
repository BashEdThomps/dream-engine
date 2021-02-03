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

#include "AssetRuntime.h"

#include "AssetDefinition.h"
#include "Common/Uuid.h"

using std::unique_lock;

namespace octronic::dream
{
    AssetRuntime::AssetRuntime
    (const string& className, AssetDefinition* definition)
      : Runtime(className, definition),
        mLoaded(false)
    {
    }

    AssetRuntime::~AssetRuntime
    ()
    {
    }

    bool
    AssetRuntime::getLoaded
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mLoaded;
    }

    void
    AssetRuntime::setLoaded
    (bool loaded)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mLoaded = loaded;
    }

    void AssetRuntime::collectGarbage()
	{
	}
}


