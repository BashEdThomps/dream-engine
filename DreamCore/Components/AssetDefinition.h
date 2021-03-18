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

#include "Base/Definition.h"
#include "Math/Transform.h"

#include <memory>

using std::weak_ptr;
using std::shared_ptr;
using std::vector;

namespace octronic::dream
{
    class ProjectDefinition;

    /**
     * @brief Base class used to define an Asset's initial properties.
     */
    class AssetDefinition : public Definition
    {

    public:
        /**
         * @brief Default Constructur
         * @param project ProjectDefinition that owns this AssetDefinition
         * @param data JSON object from which to construct the AssetDefinition
         */
        AssetDefinition(const string& className, const weak_ptr<ProjectDefinition>& project, const json& data);
        virtual ~AssetDefinition();

        /**
         * @return Returns the Dream::AssetType of the AssetDefinition
         * @see Dream::AssetType
         */
        AssetType getAssetType() const;

        /**
         * @return Get the pointer to the parant ProjectDefinition
         */
        weak_ptr<ProjectDefinition> getProject() const;

        /**
         * @brief Set the type from a string
         */
        void setType(const string&);

        /**
         * @return Get the type as a string
         */
        string getType() const;

        void setFormat(const string&);
        string getFormat() const;

        string getGroup() const;
        void setGroup(const string& group);

        weak_ptr<AssetDefinition> duplicate();

        bool isTypeAnimation() const;
        bool isTypeAudio() const;
        bool isTypeFont() const;
        bool isTypeMaterial() const;
        bool isTypeModel() const;
        bool isTypeScript() const;
        bool isTypeShader() const;
        bool isTypePath() const;
        bool isTypePhysicsObject() const;
        bool isTypeTexture() const;
    protected:
        weak_ptr<ProjectDefinition> mProjectDefinition;
    };
}
