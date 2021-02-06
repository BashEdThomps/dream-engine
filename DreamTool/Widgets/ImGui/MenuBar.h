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
        // File menu
        void checkNewProjectFailed();
        void checkOpenProjectFailed();
        void checkFileQuitClicked();
        void checkFilePreferencesClicked();
        void drawFileMenu();
        void onFileNewClicked();
        void onFileOpenClicked();
        void onFileSaveClicked();
        void onFileCloseClicked();

        void drawViewMenu();
        void drawComponentsMenu();
        void drawToolsMenu();
        void drawSceneMenu();
        void drawDebugMenu();

        void drawStatusText();


    private:
        bool mOpenProjectFailed;
        bool mNewProjectFailed;
        bool mFileQuitClicked;
        bool mFilePreferencesClicked;
        string mMessageString;
        float mMessagePadding;
    };
}
