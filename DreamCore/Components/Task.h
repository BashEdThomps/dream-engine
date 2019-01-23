#pragma once
#include "../Common/DreamObject.h"

namespace Dream
{
    class Task : public DreamObject
    {
    public:
        Task();
        virtual ~Task();
        virtual bool execute() = 0;
    };
}
