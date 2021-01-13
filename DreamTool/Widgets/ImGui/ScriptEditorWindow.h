#pragma once

#include "ImGuiWidget.h"
#include <TextEditor.h>
#include <DreamCore.h>

using Dream::ScriptDefinition;

namespace DreamTool
{
    class ScriptEditorWindow : public ImGuiWidget
    {
    public:
        ScriptEditorWindow(DTContext* state);
        ~ScriptEditorWindow() override;

        void draw() override;
        void setScriptDefinition(ScriptDefinition* scriptDefinition);

    private:
        ScriptDefinition* mScriptDefinition;
        TextEditor mTextEditor;
    };
}
