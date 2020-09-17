#pragma once

#include "GLWidget.h"

namespace Dream
{
    class ActorRuntime;
}

using namespace Dream;

namespace DreamTool
{
    class SelectionHighlighter
            : public GLWidget
    {
    public:
        SelectionHighlighter(DTState* project);
        ~SelectionHighlighter() override;
        void setSelectedActor(ActorRuntime* selected);
        void draw() override;
        void init();
        void updateGeometry();
        bool getOutlineOnly() const;
        void setOutlineOnly(bool outlineOnly);
        void clearSelection();

    protected:
        ActorRuntime* mSelectedActorRuntime;
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
