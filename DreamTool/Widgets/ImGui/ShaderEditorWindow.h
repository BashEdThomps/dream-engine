#pragma once

#include "ImGuiWidget.h"
#include <TextEditor.h>

namespace octronic::dream
{
    class ShaderDefinition;
}

namespace octronic::dream::tool
{
    class ShaderEditorWindow : public ImGuiWidget
    {
    public:
        ShaderEditorWindow(DreamToolContext* state);
        ~ShaderEditorWindow() override;

        void draw() override;
        void setShaderDefinition(ShaderDefinition* scriptDefinition);

    private:
        ShaderDefinition* mShaderDefinition;
        TextEditor mVertexEditor;
        TextEditor mFragmentEditor;
    };
}
