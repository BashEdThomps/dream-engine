#pragma once

#include "ImGuiWidget.h"
#include "PropertyType.h"

#include <glm/vec3.hpp>

namespace Dream
{
    class IDefinition;
    class IRuntime;
    class CompoundChildDefinition;
    class IAssetDefinition;
}

using namespace glm;
using namespace Dream;

namespace DreamTool
{
    class SelectionHighlighter;

    struct PropertiesTarget
    {
        PropertyType type;
        IDefinition* definition;
        IRuntime* runtime;
    };

    class PropertiesWindow : public ImGuiWidget
    {
    public:
        PropertiesWindow(DTState*);
        ~PropertiesWindow() override;

        void draw() override;
        void pushPropertyTarget(PropertyType, IDefinition*, IRuntime*);
        void popPropertyTarget();
        void clearPropertyTargets();
        void removeFromHistory(IDefinition* def);

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

        void setRuntime(IRuntime* runt);
        void setDefinition(IDefinition* def);
        void setPropertyType(PropertyType t);
        void clear();
        void drawPhysicsImGizmo(CompoundChildDefinition);
        void replaceRuntimeInstances(IAssetDefinition* assetDef);
    private:
        vector<PropertiesTarget> mHistory;
        PropertyType mType;
        IDefinition* mDefinition;
        IRuntime* mRuntime;
        ImVec2 mImageSize;
        ImVec2 mBigEditorSize;
        bool mGizmoUseSnap;
        vec3 mGizmoSnap;
    };
}
