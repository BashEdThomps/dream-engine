#include "Task.h"

namespace Dream
{
    Task::Task()
        : DreamObject("Task"),
          mExpired(false)
    {}

    Task::~Task() {}

    bool Task::hasExpired() const
    {
        return mExpired;
    }

    void Task::setExpired(bool e)
    {
        mExpired = e;
    }
}

