/*
* Dream::AssetDefinition
*
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

#include <string>
#include <map>
#include <string>
#include <vector>
#include <json.hpp>

#include "../Common/IDefinition.h"
#include "../Common/Constants.h"
#include "../Components/Transform3D.h"

using std::string;
using std::vector;
using nlohmann::json;

namespace Dream
{
    class CompoundChildDefinition
    {
    public:
       Transform3D transform;
       string      uuid;

       json getJson()
       {
           json retval = json::object();
           retval[Constants::UUID] = uuid;
           retval[Constants::TRANSFORM] = transform.getJson();
           return retval;
       }
    };

    class ProjectDefinition;

    class AssetDefinition : public IDefinition
    {

    public:
        static AssetType getAssetType(json);
        AssetDefinition(ProjectDefinition*, json);
        virtual ~AssetDefinition();

        ProjectDefinition *getProjectHandle();
        void showStatus();

        void setType(string);
        string getType();

        void setFormat(string);
        string getFormat();

        string getAssetTypeDirectory();

        bool isTypeAnimation();
        bool isTypeAudio();
        bool isTypeModel();
        bool isTypeScript();
        bool isTypeShader();
        bool isTypePhysicsObject();
        bool isTypeLight();
        bool isTypeSprite();
        bool isTypeFont();

        string getAssetPath();

        void setProjectPath(string);
        string getProjectPath();

        // Common
        void  setColourRed(float);
        float getColourRed();

        void  setColourGreen(float);
        float getColourGreen();

        void  setColourBlue(float);
        float getColourBlue();

        void  setColourAlpha(float);
        float getColourAlpha();

        void  setSize(float);
        float getSize();

        // Animation

        // Audio
        bool isAudioFormatWav();
        bool isAudioFormatOgg();

        void setAudioLoop(bool);
        bool getAudioLoop();

        // Model
        bool isModelFormatAssimp();

        // Script
        bool isScriptFormatLua();

        // Physics
        void  setPhysicsObjectMass(float mass);
        float getPhysicsObjectMass();

        void  setPhysicsObjectMargin(float margin);
        float getPhysicsObjectMargin();

        void setPhysicsObjectKinematic(bool kinematic);
        bool getPhysicsObjectKinematic();

        void  setPhysicsObjectHalfExtentsX(float halfExtentX);
        float getPhysicsObjectHalfExtentsX();

        void  setPhysicsObjectHalfExtentsY(float halfExtentY);
        float getPhysicsObjectHalfExtentsY();

        void  setPhysicsObjectHalfExtentsZ(float halfExtentZ);
        float getPhysicsObjectHalfExtentsZ();

        void  setPhysicsObjectNormalX(float normalX);
        float getPhysicsObjectNormalX();

        void  setPhysicsObjectNormalY(float normalY);
        float getPhysicsObjectNormalY();

        void  setPhysicsObjectNormalZ(float normalZ);
        float getPhysicsObjectNormalZ();

        float getPhysicsObjectRadius();
        void  setPhysicsObjectRadius(float rad);

        float getPhysicsObjectConstant();
        void  setPhysicsObjectConstant(float constant);

        void addPhysicsObjectCompoundChild(CompoundChildDefinition child);
        vector<CompoundChildDefinition> getPhysicsObjectCompoundChildren();
        void removePhysicsObjectCompoundChild(CompoundChildDefinition def);

    protected:
        ProjectDefinition *mProjectDefinitionHandle;

        void makeCompoundChildren();
        void makeHalfExtentsObject();
        void makeLightColourObject();
        void makeNormalObject();

    }; // End of AssetDefinition

} // End of Dream
