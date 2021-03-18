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
        shared_ptr<Definition> definition;
        shared_ptr<Runtime> runtime;
    };

    class PropertiesWindow : public ImGuiWidget
    {
    public:
        PropertiesWindow(DreamToolContext*);
        ~PropertiesWindow() override;

        void draw() override;
        void pushPropertyTarget(PropertyType, const shared_ptr<Definition>&, const shared_ptr<Runtime>&);
        void popPropertyTarget();
        void clearPropertyTargets();
        void removeFromHistory(const shared_ptr<Definition>& def);

    protected:
        void drawImGizmo();
        void drawTransformProperties();
        int getStringIndexInVector(const string& str, const vector<string>& vec);
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
        void drawMaterialAssetProperties();
        void drawModelAssetProperties();
        void drawPhysicsObjectAssetProperties();
        void drawScriptProperties();
        void drawShaderAssetProperties();
        void drawPathAssetProperties();
        void drawTextureAssetProperties();

        void setRuntime(const shared_ptr<Runtime> runt);
        void setDefinition(const shared_ptr<Definition>& def);
        void setPropertyType(PropertyType t);
        void clear();
        void replaceRuntimes(const shared_ptr<AssetDefinition>& assetDef);
    private:
        vector<PropertiesTarget> mHistory;
        PropertyType mType;
        shared_ptr<Definition> mDefinition;
        shared_ptr<Runtime> mRuntime;
        ImVec2 mImageSize;
    };
}
