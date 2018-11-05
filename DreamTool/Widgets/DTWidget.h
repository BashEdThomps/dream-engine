//
//  DTWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once

#include <DreamCore.h>

using namespace Dream;

namespace DreamTool
{
    class DTWidget : public DreamObject
    {
    public:
        DTWidget(Project* project);
        virtual ~DTWidget();
        virtual void draw() = 0;

        bool getHidden() const;
        void setHidden(bool hidden);

    protected:
        Project* mProject;
        bool mHidden;
    };
}
