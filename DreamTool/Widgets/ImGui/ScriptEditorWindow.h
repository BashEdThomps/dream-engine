#pragma once

#include "ImGuiWidget.h"
#include "../../deps/ImTextEditor/TextEditor.h"


namespace Dream
{
    class ScriptDefinition;
}

namespace DreamTool
{
    class ScriptEditorWindow : public ImGuiWidget
    {
    public:
        ScriptEditorWindow(DTState* state);
        ~ScriptEditorWindow() override;

        void draw() override;
        void setScriptDefinition(ScriptDefinition* scriptDefinition);

    private:
        ScriptDefinition* mScriptDefinition;
        TextEditor mTextEditor;
    };
}
