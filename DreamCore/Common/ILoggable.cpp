#include "ILoggable.h"

namespace Dream
{
    ILoggable::ILoggable(string classname) :_CLASSNAME_(classname) {}

    ILoggable::~ILoggable() {}

    void ILoggable::setLogClassName(string name)
    {
       _CLASSNAME_ = name;

    }

    string ILoggable::getClassName()
    {
        return _CLASSNAME_;
    }

    std::shared_ptr<spdlog::logger> ILoggable::getLog() const
    {
        auto log = spdlog::get(_CLASSNAME_);
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt(_CLASSNAME_);
        }
        return log;
    }
}
