//
//  DTWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once

#include "../../DreamCore/Common/DreamObject.h"

using namespace Dream;

namespace DreamTool
{
    class DTState;
    class DTWidget : public DreamObject
    {
    public:
        DTWidget(DTState* state, bool visible = true);
        virtual ~DTWidget();
        virtual void draw() = 0;

        bool getVisible() const;
        void setVisible(bool);

    protected:
        DTState* mState;
        bool mVisible;
    };
}
