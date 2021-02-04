#pragma once

#include "ImGuiWidget.h"
#include "PropertyType.h"
#include "glm/vec3.hpp"
#include <DreamCore.h>

using octronic::dream::Definition;
using octronic::dream::Runtime;
using octronic::dream::PathDefinition;
using octronic::dream::AnimationDefinition;
using octronic::dream::CompoundChildDefinition;
using octronic::dream::AssetDefinition;
using octronic::dream::AnimationKeyframe;
using octronic::dream::PathControlPoint;


namespace octronic::dream::tool
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
        PropertiesWindow(DreamToolContext*);
        ~PropertiesWindow() override;

        void draw() override;
        void pushPropertyTarget(PropertyType, Definition*, Runtime*);
        void popPropertyTarget();
        void clearPropertyTargets();
        void removeFromHistory(Definition* def);

    protected:
        void drawImGizmo();
        void drawNonImGizmo();
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
        void drawTextureAssetProperties();
        void drawSpriteAssetProperties();

        void setRuntime(Runtime* runt);
        void setDefinition(Definition* def);
        void setPropertyType(PropertyType t);
        void clear();
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
