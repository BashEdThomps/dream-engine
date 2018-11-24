#pragma once

#include "ImGuiWidget.h"
#include "../../Model/ModelDefinitionBatchImporter.h"

namespace Dream
{
    class Project;
}

using namespace Dream;

namespace DreamTool
{
    class LuaDebugWindow;
    class ProjectBrowser;
    class PropertiesWindow;
    class SceneStateWindow;
    class GridPropertiesWindow;
    class SelectionHighlighter;

    class MenuBar : public ImGuiWidget
    {
    public:
        MenuBar(DTState*);
        ~MenuBar() override;
        void draw() override;

        float getMessagePadding() const;
        void setMessagePadding(float messagePadding);

        string getMessageString() const;
        void setMessageString(const string& messageString);

        int getFPS() const;
        void setFPS(int fPS);

    protected:
        string mMessageString;
        float mMessagePadding = 15.0f;
        int mFPS;
    };
}
