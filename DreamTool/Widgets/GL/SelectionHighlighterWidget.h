#pragma once

#include "GLWidget.h"

namespace DreamTool
{
    class SelectionHighlighterWidget
            : public GLWidget
    {
    public:
        SelectionHighlighterWidget(Dream::Project* project);
        ~SelectionHighlighterWidget() override;
        void setSelectedSceneObject(SceneObjectRuntime* selected);
        void draw() override;
        void init();
        void updateGeometry();
        bool getOutlineOnly() const;
        void setOutlineOnly(bool outlineOnly);
        void clearSelection();

    protected:
        SceneObjectRuntime* mSelectedSceneObjectRuntime;
        vec3 mSelectionColour;
        float mOffset;
        vec3 mXColour;
        vec3 mYColour;
        vec3 mZColour;

        // GLWidget interface
    protected:
        void setShader() override;
        bool mOutlineOnly;
    };
}
