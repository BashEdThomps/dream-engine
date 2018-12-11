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
        AssetDefinition(ProjectDefinition* project, json data);
        AssetType getAssetType();

        virtual ~AssetDefinition();

        ProjectDefinition* getProject();

        void setType(string);
        string getType();

        void setFormat(string);
        string getFormat();

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

        string getGroup();
        void setGroup(string group);

    protected:
        ProjectDefinition* mProjectDefinition;

    }; // End of AssetDefinition

} // End of Dream
