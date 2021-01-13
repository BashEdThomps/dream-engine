#pragma once

#include "ImGuiWidget.h"
#include "PropertyType.h"
#include "glm/vec3.hpp"
#include <DreamCore.h>

using Dream::Definition;
using Dream::Runtime;
using Dream::PathDefinition;
using Dream::AnimationDefinition;
using Dream::CompoundChildDefinition;
using Dream::AssetDefinition;
using Dream::AnimationKeyframe;
using Dream::PathControlPoint;


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
        PropertiesWindow(DTContext*);
        ~PropertiesWindow() override;

        void draw() override;
        void pushPropertyTarget(PropertyType, Definition*, Runtime*);
        void popPropertyTarget();
        void clearPropertyTargets();
        void removeFromHistory(Definition* def);

    protected:
        void drawImGizmo();
        void drawPathControlPointImGuizmo(PathDefinition* pDef, PathControlPoint);
        void drawAnimationKeyframeImGuizmo(AnimationDefinition* pDef, AnimationKeyframe);
        int getStringIndexInVector(string str, vector<string> vec);
        bool drawDeleteEntityButton();
        bool drawDeleteSceneButton();
        void drawNameAndIdProperties();
        void drawProjectProperties();
        void drawSceneProperties();
        void drawEntityProperties();
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
        void drawScrollerAssetProperties();
        void drawObjectEmitterProperties();

        void setRuntime(Runtime* runt);
        void setDefinition(Definition* def);
        void setPropertyType(PropertyType t);
        void clear();
        void drawPhysicsImGizmo(CompoundChildDefinition);
        void replaceRuntimes(AssetDefinition* assetDef);
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
