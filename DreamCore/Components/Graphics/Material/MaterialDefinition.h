/*
 * MaterialDefinition.h
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

#include "../../AssetDefinition.h"

namespace Dream
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
        MaterialDefinition(ProjectDefinition* pd, json js);
        ~MaterialDefinition() override;

        // Shader
        string getShader();
        void setShader(string);

        // Textures
        string getDiffuseTexture();
        void setDiffuseTexture(string);

        string getSpecularTexture();
        void setSpecularTexture(string);

        string getNormalTexture();
        void setNormalTexture(string);

        string getDisplacementTexture();
        void setDisplacementTexture(string);

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

