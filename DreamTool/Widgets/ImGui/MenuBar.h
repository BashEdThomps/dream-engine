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
    class ToolPropertiesWindow;
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

        void setFPS(double fPS);

    protected:
        string mMessageString;
        float mMessagePadding = 15.0f;
        double mFPS;
    };
}
