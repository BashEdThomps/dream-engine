#include "ProjectBrowser.h"

#include <DreamCore.h>
#include "../deps/ImGui/imgui.h"

namespace DreamTool
{
    ProjectBrowser::ProjectBrowser
    (Project* project)
        : DTWidget(project)
    {}
    ProjectBrowser::~ProjectBrowser
    ()
    {
        setLogClassName("ProjectBrowser");
    }

    void
    ProjectBrowser::draw
    ()
    {
        auto log = getLog();
        char buf[512];
        float f;
        ImGui::Text(
            "%s Project Browser",
            mProject->getProjectDefinition()->getName().c_str()
        );
        if (ImGui::Button("Save"))
        {
            log->debug("Save button was clicked");
        }
        ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    }
}
