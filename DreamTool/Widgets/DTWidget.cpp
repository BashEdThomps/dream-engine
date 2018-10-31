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
    (Project* project)
    : DreamObject("DTWidget"),
      mProject(project),
      mTreeID(0),
      mHidden(false)
    {}

    DTWidget::~DTWidget() {}

    bool DTWidget::getHidden() const
    {
        return mHidden;
    }

    void DTWidget::setHidden(bool hidden)
    {
        mHidden = hidden;
    }
}
