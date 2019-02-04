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
#include "AssetRuntime.h"

namespace Dream
{
    class SceneObjectRuntime;
    class Task;

    /**
     * @brief A DiscreteAssetRuntime is unique to a single SceneObjectRuntime
     */
    class DiscreteAssetRuntime : public AssetRuntime
    {
    public:
        /**
         * @param def Definition from which to create Runtime.
         * @param runtime Owner of this AssetRuntime.
         */
        DiscreteAssetRuntime(AssetDefinition* def, SceneObjectRuntime* runtime);
        ~DiscreteAssetRuntime() override;

        SceneObjectRuntime* getSceneObjectRuntime();
        string getAssetFilePath(string fmt = "") override;
        string getAssetDirectoryPath() override;

        void setUpdateTask(Task*);
        bool hasUpdateTask();

    protected:
        SceneObjectRuntime* mSceneObjectRuntime;
        Task* mUpdateTask;

    };
}
