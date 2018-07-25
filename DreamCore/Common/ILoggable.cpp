#include "ILoggable.h"

namespace Dream
{
    ILoggable::ILoggable(string classname) :_CLASSNAME_(classname) {}

    ILoggable::~ILoggable() {}

    std::shared_ptr<spdlog::logger> ILoggable::getLog()
    {
        auto log = spdlog::get(_CLASSNAME_);
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt(_CLASSNAME_);
        }
        return log;
    }
}
