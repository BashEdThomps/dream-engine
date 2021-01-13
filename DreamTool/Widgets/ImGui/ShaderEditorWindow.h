#pragma once

#include "ImGuiWidget.h"
#include <TextEditor.h>

namespace Dream
{
    class ShaderDefinition;
}

using namespace Dream;

namespace DreamTool
{
    class ShaderEditorWindow : public ImGuiWidget
    {
    public:
        ShaderEditorWindow(DTContext* state);
        ~ShaderEditorWindow() override;

        void draw() override;
        void setShaderDefinition(ShaderDefinition* scriptDefinition);

    private:
        ShaderDefinition* mShaderDefinition;
        TextEditor mVertexEditor;
        TextEditor mFragmentEditor;
    };
}
