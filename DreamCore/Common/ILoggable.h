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
        void setLogClassName(string);
        string getClassName();
    protected:
        std::shared_ptr<spdlog::logger> getLog() const;
        string  _CLASSNAME_;
    };
}
