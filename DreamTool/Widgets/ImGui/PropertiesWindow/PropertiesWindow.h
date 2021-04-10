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

#include <memory>

using std::unique_ptr;
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
        void pushPropertyTarget(PropertyType type, optional<reference_wrapper<Definition>> def);
        void popPropertyTarget();
        optional<reference_wrapper<PropertyTarget>> getCurrentPropertyTarget();
        void clearPropertyTargets();
        void removeFromHistory(Definition& def);
        void removeFromHistoryWithChildren(SceneEntityDefinition& def);

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
        vector<unique_ptr<PropertyTarget>> mHistory;
        ImVec2 mImageSize;
    };
}
