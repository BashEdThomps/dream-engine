/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#pragma once

#include "Common/Math.h"
#include "Components/AssetDefinition.h"

namespace octronic::dream
{

    class MaterialDefinition : public AssetDefinition
    {
    public:
        MaterialDefinition(ProjectDefinition* pd, const json& js);
        ~MaterialDefinition() override;

        // Shader
        UuidType getShader();
        void setShader(UuidType);

        // Textures
        UuidType getDiffuseTexture();
        void setDiffuseTexture(UuidType);

        UuidType getSpecularTexture();
        void setSpecularTexture(UuidType);

        UuidType getNormalTexture();
        void setNormalTexture(UuidType);

        UuidType getDisplacementTexture();
        void setDisplacementTexture(UuidType);

        // Colour
        Vector3 getDiffuseColour();
        void setDiffuseColour(Vector3);

        Vector3 getSpecularColour();
        void setSpecularColour(Vector3);

        Vector3 getAmbientColour();
        void setAmbientColour(Vector3);

        Vector3 getReflectiveColour();
        void setReflectiveColour(Vector3);

        Vector3 getEmissiveColour();
        void setEmissiveColour(Vector3);

        float getOpacity();
        void  setOpacity(float);

        float getBumpScaling();
        void  setBumpScaling(float);

        float getHardness();
        void  setHardness(float);

        void  setReflectivity(float);
        float getReflectivity();

        float getShininessStrength();
        void  setShininessStrength(float);

        float getRefractionIndex();
        void  setRefractionIndex(float);

        bool getIgnore();
        void setIgnore(bool);

    private:
        json black();

    };
}

