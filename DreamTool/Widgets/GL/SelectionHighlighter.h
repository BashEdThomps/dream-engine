#pragma once

#include "GLWidget.h"

namespace Dream
{
    class EntityRuntime;
}

using namespace Dream;

namespace DreamTool
{
    class SelectionHighlighter
            : public GLWidget
    {
    public:
        SelectionHighlighter(DTContext* project);
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
