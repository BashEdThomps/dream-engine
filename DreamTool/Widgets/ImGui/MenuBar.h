#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream
{
    class Project;
}

namespace octronic::dream::tool
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
        MenuBar(DreamToolContext*);
        ~MenuBar() override;
        void draw() override;

        float getMessagePadding() const;
        void setMessagePadding(float messagePadding);

        string getMessageString() const;
        void setMessageString(const string& messageString);
    private:
        void drawFileMenu();
        void onFileNewClicked();
        void onFileOpenClicked();
        void onFileQuitClicked();
        void onFileSaveClicked();
        void onFileCloseClicked();

        void drawViewMenu();
        void drawComponentsMenu();
        void drawToolsMenu();
        void drawSceneMenu();
        void drawDebugMenu();

        void drawStatusText();

    protected:
        string mMessageString;
        float mMessagePadding = 15.0f;
    };
}
