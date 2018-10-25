#include "PropertiesWindow.h"

namespace DreamTool
{
    PropertiesWindow::PropertiesWindow
    (Dream::Project* proj)
        : DTWidget (proj)
    {
        setLogClassName("Properties Window");
    }

    PropertiesWindow::~PropertiesWindow()
    {

    }

    void
    PropertiesWindow::draw
    ()
    {
        ImGui::Begin("Properties");
        ImGui::Text("");
        ImGui::Text("");
        ImGui::End();
    }

    void
    PropertiesWindow::setDefinition
    (IDefinition* def)
    {

    }

    void
    PropertiesWindow::setRuntime
    (IRuntime* runt)
    {

    }
}
