//
//  DTWidget.cpp
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#include "DreamToolWidget.h"

namespace octronic::dream::tool
{
    DreamToolWidget::DreamToolWidget(DreamToolContext* ctx, bool visible)
    : mContext(ctx),
      mVisible(visible)
    {

    }

    DreamToolWidget::~DreamToolWidget()
    {

    }

    bool
    DreamToolWidget::getVisible
    ()
    const
    {
        return mVisible;
    }

    void
    DreamToolWidget::setVisible
    (bool v)
    {
        mVisible = v;
    }
}
