#pragma once
#include "../../Common/DreamObject.h"

namespace Dream
{
    class Task : public DreamObject
    {
    protected:
        bool mExpired;

    public:
        Task() :
            DreamObject("Task"),
            mExpired(false)
        {}

        virtual ~Task() {}
        virtual bool execute() = 0;

        inline bool
        hasExpired
        () const
        {
            return mExpired;
        }

        inline void
        setExpired
        (bool e)
        {
            mExpired = e;
        }
    };
}
