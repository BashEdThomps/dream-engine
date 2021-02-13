#include "TaskManagerWindow.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

namespace octronic::dream::tool
{
    TaskManagerWindow::TaskManagerWindow
    (DreamToolContext* proj) : ImGuiWidget(proj,false)
    {
    }

    TaskManagerWindow::~TaskManagerWindow
    ()
    {

    }

    void
    TaskManagerWindow::draw
    ()
    {
        ImGui::Begin("Task Manager",&mVisible);

        if (mContext->getProject())
        {
            auto projRunt = mContext->getProject()->getRuntime();
            if (projRunt)
            {
            }
            else
            {
                ImGui::Text("No Project Runtime");
            }
        }
        else
        {
                ImGui::Text("No Project");
        }
        ImGui::End();
    }
}
