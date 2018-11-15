#pragma once

#include "ImGuiWidget.h"
#include "../../deps/ImGui/TextEditor.h"

namespace DreamTool
{
    class ShaderEditorWindow : public ImGuiWidget
    {
    public:
        ShaderEditorWindow(DTState* state);
        ~ShaderEditorWindow() override;

        void draw() override;
        void setShaderDefinition(ShaderDefinition* scriptDefinition);

    private:
        ShaderDefinition* mShaderDefinition;
        TextEditor mVertexEditor;
        TextEditor mFragmentEditor;
    };
}
