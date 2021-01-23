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

namespace octronic::dream
{
    class EntityRuntime;

    /**
     * @brief A DiscreteAssetRuntime is unique to a single EntityRuntime
     */
    class DiscreteAssetRuntime : public AssetRuntime
    {
    public:
        /**
         * @param def Definition from which to create Runtime.
         * @param runtime Owner of this AssetRuntime.
         */
        DiscreteAssetRuntime(AssetDefinition* def, EntityRuntime* runtime);
        ~DiscreteAssetRuntime() override;

        EntityRuntime* getEntityRuntime();
        string getAssetFilePath(string fmt = "") override;
        string getAssetDirectoryPath() override;

    protected:
        EntityRuntime* mEntityRuntime;
    };
}
