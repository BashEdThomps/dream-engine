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
#include "Project/ProjectRuntime.h"
#include "Storage/ProjectDirectory.h"
#include "Common/Uuid.h"

using std::static_pointer_cast;

namespace octronic::dream
{
    AssetRuntime::AssetRuntime
    (const weak_ptr<ProjectRuntime>& pr,
     const weak_ptr<AssetDefinition>& definition)
        : DeferredLoadRuntime(pr, definition)

    {
    }

    AssetRuntime::~AssetRuntime
    ()
    {
    }

    string
    AssetRuntime::getAssetFilePath
    (const string& fmt)
    {
        if (auto prLock = mProjectRuntime.lock())
        {
            if (auto pDirLock = prLock->getProjectDirectory().lock())
            {
                if (auto defLock = mDefinition.lock())
                {
            		return pDirLock->getAssetAbsolutePath(static_pointer_cast<AssetDefinition>(defLock),fmt);
                }
            }
        }
        return "";
    }

    string
    AssetRuntime::getAssetDirectoryPath
    ()
    {
        if (auto prLock = mProjectRuntime.lock())
        {
            if (auto pDirLock = prLock->getProjectDirectory().lock())
            {
                if (auto defLock = mDefinition.lock())
                {
        			return pDirLock->getAssetDirectoryPath(static_pointer_cast<AssetDefinition>(defLock));
                }
            }
        }
        return "";
    }
}


