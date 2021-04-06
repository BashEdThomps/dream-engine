#pragma once

#include <DreamCore.h>

namespace octronic::dream::tool
{
    class DreamToolContext;
    class DreamToolWidget
    {
    public:
        DreamToolWidget(DreamToolContext& ctx, bool visible = true);
        virtual ~DreamToolWidget();

        virtual void draw() = 0;

        bool getVisible() const;
        void setVisible(bool);

        DreamToolContext& getContext() const;
    protected:
        bool mVisible;

    private:
        reference_wrapper<DreamToolContext> mContext;
    };
}
