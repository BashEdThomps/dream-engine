#pragma once

#include "ImGuiWidget.h"
#include <TextEditor.h>
#include <DreamCore.h>

using octronic::dream::ScriptDefinition;

namespace octronic::dream::tool
{
    class ScriptEditorWindow : public ImGuiWidget
    {
    public:
        ScriptEditorWindow(DreamToolContext* state);
        ~ScriptEditorWindow() override;

        void draw() override;
        void setScriptDefinition(ScriptDefinition* scriptDefinition);

    private:
        ScriptDefinition* mScriptDefinition;
        TextEditor mTextEditor;
    };
}
