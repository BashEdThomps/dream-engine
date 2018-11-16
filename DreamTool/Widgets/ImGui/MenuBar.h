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
    };
}
