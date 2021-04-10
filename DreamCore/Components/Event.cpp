#include "Event.h"

#include "Common/Logger.h"
#include "Entity/EntityRuntime.h"

namespace octronic::dream
{
    Event::Event
    (const map<string,string>& attributes)
        : mAttributes(attributes),
          mProcessed(false)
    {
        LOG_TRACE("Event: Creating");
    }

    Event::~Event()
    {
        LOG_TRACE("Event: Destroying");
    }

    void
    Event::setProcessed
    (bool p)
    {
        mProcessed = p;
    }

    bool
    Event::getProcessed
    ()
    const
    {
       return mProcessed;
    }

    string
    Event::getAttribute
    (const string &key) const
    {
        if (mAttributes.count(key) > 0)
        {
            return mAttributes.at(key);
        }
        return "";
    }

    void
    Event::setAttribute
    (const string &key, const string &value)
    {
        mAttributes[key] = value;
    }
}
