#pragma once
#include "../Common/DreamObject.h"

namespace Dream
{
    class Task : public DreamObject
    {
    protected:
        bool mExpired;

    public:
        Task();
        virtual ~Task();
        virtual bool execute() = 0;

        bool hasExpired() const;
        void setExpired(bool executed);
    };
}
