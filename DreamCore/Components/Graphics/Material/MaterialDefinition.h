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

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    struct RGB
    {
        float r;
        float g;
        float b;
    };

    class MaterialDefinition : public AssetDefinition
    {
    public:
        MaterialDefinition(ProjectDefinition* pd, const json& js);
        ~MaterialDefinition() override;

        // Shader
        uint32_t getShader();
        void setShader(uint32_t);

        // Textures
        uint32_t getDiffuseTexture();
        void setDiffuseTexture(uint32_t);

        uint32_t getSpecularTexture();
        void setSpecularTexture(uint32_t);

        uint32_t getNormalTexture();
        void setNormalTexture(uint32_t);

        uint32_t getDisplacementTexture();
        void setDisplacementTexture(uint32_t);

        // Colour
        RGB getDiffuseColour();
        void setDiffuseColour(RGB);

        RGB getSpecularColour();
        void setSpecularColour(RGB);

        RGB getAmbientColour();
        void setAmbientColour(RGB);

        RGB getReflectiveColour();
        void setReflectiveColour(RGB);

        RGB getEmissiveColour();
        void setEmissiveColour(RGB);

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
        json wrapColourObject(RGB);
        RGB unwrapColourObject(json);
        json black();

    };
}

