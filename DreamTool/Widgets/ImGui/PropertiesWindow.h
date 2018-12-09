#pragma once

#include "ImGuiWidget.h"
#include "PropertyType.h"

#include <glm/vec3.hpp>

namespace Dream
{
    class Definition;
    class Runtime;
    class CompoundChildDefinition;
    class AssetDefinition;
}

using namespace glm;
using namespace Dream;

namespace DreamTool
{
    class SelectionHighlighter;

    struct PropertiesTarget
    {
        PropertyType type;
        Definition* definition;
        Runtime* runtime;
    };

    class PropertiesWindow : public ImGuiWidget
    {
    public:
        PropertiesWindow(DTState*);
        ~PropertiesWindow() override;

        void draw() override;
        void pushPropertyTarget(PropertyType, Definition*, Runtime*);
        void popPropertyTarget();
        void clearPropertyTargets();
        void removeFromHistory(Definition* def);

    protected:
        void drawImGizmo();
        int getStringIndexInVector(string str, vector<string> vec);
        bool drawDeleteSceneObjectButton();
        bool drawDeleteSceneButton();
        void drawNameAndIdProperties();
        void drawProjectProperties();
        void drawSceneProperties();
        void drawSceneObjectProperties();
        void drawAssetProperties();
        void drawAnimationAssetProperties();
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

        void setRuntime(Runtime* runt);
        void setDefinition(Definition* def);
        void setPropertyType(PropertyType t);
        void clear();
        void drawPhysicsImGizmo(CompoundChildDefinition);
        void replaceRuntimeInstances(AssetDefinition* assetDef);
    private:
        vector<PropertiesTarget> mHistory;
        PropertyType mType;
        Definition* mDefinition;
        Runtime* mRuntime;
        ImVec2 mImageSize;
        ImVec2 mBigEditorSize;
        bool mGizmoUseSnap;
        vec3 mGizmoSnap;
    };
}
