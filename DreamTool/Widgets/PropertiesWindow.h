#pragma once

#include "DTWidget.h"
#include "PropertyType.h"

using Dream::IDefinition;
using Dream::IRuntime;

#define BigEditorBufferSize 1024*1024

namespace DreamTool
{
    struct PropertiesTarget
    {
        PropertyType type;
        IDefinition* definition;
        IRuntime* runtime;
    };

    class PropertiesWindow : public DTWidget
    {
    public:
        PropertiesWindow(Project*);
        ~PropertiesWindow() override;

        void draw() override;
        void pushPropertyTarget(PropertyType, IDefinition*, IRuntime*);
        void popPropertyTarget();

    private:
        int getStringIndexInVector(string str, vector<string> vec);
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

        void setRuntime(IRuntime* runt);
        void setDefinition(IDefinition* def);
        void setPropertyType(PropertyType t);
        void clear();
    private:
        vector<PropertiesTarget> mHistory;
        PropertyType mType;
        IDefinition* mDefinition;
        IRuntime* mRuntime;
        ImVec2 mImageSize;
        ImVec2 mBigEditorSize;
    };

}