#pragma once

#include "DTWidget.h"
#include "PropertyType.h"

using Dream::IDefinition;
using Dream::IRuntime;

namespace DreamTool
{
    class PropertiesWindow : public DTWidget
    {
    public:
        PropertiesWindow(Project*);
        ~PropertiesWindow() override;

        void draw() override;
        void setRuntime(IRuntime* runt);
        void setDefinition(IDefinition* def);
        void clear();
        void setPropertyType(PropertyType t);

    private:
        void drawNameAndIdProperties();
        void drawProjectProperties();
        void drawSceneProperties();
        void drawSceneObjectProperties();
        void drawAssetProperties();
        void drawAudioAssetProperties();
        void drawFontAssetProperties();
        void drawLightAssetProperties();
        void drawMaterialAssetProperties();
        void drawModelAssetProperties();
        void drawPhysicsObjectAssetProperties();
        void drawScriptProperties();
        void drawShaderAssetProperties();
        void drawPathAssetProperties();
        void drawParticleEmitterAssetProperties();
        void drawTextureAssetProperties();

    private:
        PropertyType mType;
        IDefinition* mDefinition;
        IRuntime* mRuntime;
    };

}
