#pragma once

#include <DreamCore.h>

namespace octronic::dream::tool
{
    class DreamToolContext;
    class DreamToolWidget
    {
    public:
        DreamToolWidget(DreamToolContext& ctx, bool visible = true);

        DreamToolWidget(DreamToolWidget&) = delete;
        DreamToolWidget& operator=(DreamToolWidget&) = delete;

        DreamToolWidget(DreamToolWidget&&) = default;
        DreamToolWidget& operator=(DreamToolWidget&&) = default;

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
