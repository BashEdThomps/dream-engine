#include "DreamToolWidget.h"

namespace octronic::dream::tool
{
    DreamToolWidget::DreamToolWidget
    (DreamToolContext& ctx, bool visible)
    : mVisible(visible),
      mContext(ctx)
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

    DreamToolContext&
    DreamToolWidget::getContext
    ()
    const
    {
      return mContext.get();
    }
}
