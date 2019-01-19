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

#include "../Common/Definition.h"
#include "Transform.h"

using std::vector;

namespace Dream
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
        AssetDefinition(ProjectDefinition* project, json data);
        virtual ~AssetDefinition();

        /**
         * @return Returns the Dream::AssetType of the AssetDefinition
         * @see Dream::AssetType
         */
        AssetType getAssetType();

        /**
         * @return Get the pointer to the parant ProjectDefinition
         */
        ProjectDefinition* getProject();

        /**
         * @brief Set the type from a string
         */
        void setType(string);

        /**
         * @return Get the type as a string
         */
        string getType();

        void setFormat(string);
        string getFormat();

        string getGroup();
        void setGroup(string group);

        AssetDefinition* duplicate();

        bool isTypeAnimation();
        bool isTypeAudio();
        bool isTypeFont();
        bool isTypeLight();
        bool isTypeMaterial();
        bool isTypeModel();
        bool isTypeScript();
        bool isTypeShader();
        bool isTypePath();
        bool isTypeParticleEmitter();
        bool isTypePhysicsObject();
        bool isTypeTexture();

            protected:
        ProjectDefinition* mProjectDefinition;

    };
}
