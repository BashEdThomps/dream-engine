//
//  DTWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once

#include <DreamCore.h>

namespace DreamTool
{
    class DTContext;
    class DTWidget
    {
    public:
        DTWidget(DTContext* state, bool visible = true);
        virtual ~DTWidget();
        virtual void draw() = 0;

        bool getVisible() const;
        void setVisible(bool);

    protected:
        DTContext* mState;
        bool mVisible;
    };
}
