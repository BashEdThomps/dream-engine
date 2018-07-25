#pragma once

#include <spdlog/spdlog.h>
#include <string>

using namespace std;

namespace Dream
{
    class ILoggable
    {
    public:
        ILoggable(string classname);
        virtual ~ILoggable();
        std::shared_ptr<spdlog::logger> getLog();
        string  _CLASSNAME_;
    };
}
