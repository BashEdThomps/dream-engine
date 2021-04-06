#pragma once

#include "Widgets/ImGui/ImGuiWidget.h"

#include "glm/vec3.hpp"
#include <DreamCore.h>

#include "PropertyTarget.h"
#include "ScenePropertiesPanel.h"
#include "SceneEntityPropertiesPanel.h"
#include "TemplateEntityPropertiesPanel.h"
#include "ProjectPropertiesPanel.h"
#include "AssetPropertiesPanel.h"

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

    class PropertiesWindow : public ImGuiWidget
    {
    public:
        PropertiesWindow(DreamToolContext&);

        void draw() override;
        void pushPropertyTarget(PropertyTarget);
        void popPropertyTarget();
        PropertyTarget getCurrentPropertyTarget() const;
        void clearPropertyTargets();
        void removeFromHistory(Definition& def);

        ImVec2 getImageSize() const;

    protected:
        int getStringIndexInVector(const string& str, const vector<string>& vec);
        void clear();

    private:
        ScenePropertiesPanel mScenePropertiesPanel;
        ProjectPropertiesPanel mProjectPropertiesPanel;
        SceneEntityPropertiesPanel mSceneEntityPropertiesPanel;
        TemplateEntityPropertiesPanel mTemplateEntityPropertiesPanel;
        AssetPropertiesPanel mAssetPropertiesPanel;
        vector<PropertyTarget> mHistory;
        ImVec2 mImageSize;
    };
}
