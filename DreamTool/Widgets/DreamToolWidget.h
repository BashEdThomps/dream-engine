//
//  DTWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once

#include <DreamCore.h>

namespace octronic::dream::tool
{
    class DreamToolContext;
    class DreamToolWidget
    {
    public:
        DreamToolWidget(DreamToolContext* ctx, bool visible = true);
        virtual ~DreamToolWidget();
        virtual void draw() = 0;

        bool getVisible() const;
        void setVisible(bool);

    protected:
        DreamToolContext* mContext;
        bool mVisible;
    };
}
