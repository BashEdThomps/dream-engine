//
//  DTWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once

#include <DreamCore.h>
#include "../deps/ImGui/imgui.h"

using Dream::DreamObject;
using Dream::Project;

namespace DreamTool
{
    class DTWidget : public DreamObject
    {
    public:
        DTWidget(Project* project);
        virtual ~DTWidget();
        virtual void draw() = 0;
    protected:
        Project* mProject;
        int mTreeID;
    };
}
