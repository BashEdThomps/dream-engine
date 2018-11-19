//
//  DTWidget.cpp
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#include "DTWidget.h"

namespace DreamTool
{
    DTWidget::DTWidget
    (DTState* state, bool visible)
    : DreamObject("DTWidget"),
      mState(state),
      mVisible(visible)
    {}

    DTWidget::~DTWidget
    ()
    {}

    bool
    DTWidget::getVisible
    ()
    const
    {
        return mVisible;
    }

    void
    DTWidget::setVisible
    (bool v)
    {
        mVisible = v;
    }
}
