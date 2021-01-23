#pragma once

#include "GLWidget.h"

namespace octronic::dream
{
    class EntityRuntime;
}

namespace octronic::dream::tool
{
    class SelectionHighlighter : public GLWidget
    {
    public:
        SelectionHighlighter(DreamToolContext* project);
        ~SelectionHighlighter() override;
        void setSelectedEntity(EntityRuntime* selected);
        void draw() override;
        void init();
        void updateGeometry();
        bool getOutlineOnly() const;
        void setOutlineOnly(bool outlineOnly);
        void clearSelection();

    protected:
        EntityRuntime* mSelectedEntityRuntime;
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
