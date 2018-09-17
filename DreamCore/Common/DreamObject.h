#pragma once

#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using std::enable_shared_from_this;
using std::shared_ptr;
using std::shared_ptr;
using std::string;
using namespace spdlog;

namespace Dream
{
    class DreamObject
    {
    public:
        DreamObject(string classname);
        virtual ~DreamObject();

        void setLogClassName(string);
        string getClassName();

    protected:
        shared_ptr<logger> getLog() const;
        string  _CLASSNAME_;

    };
}
