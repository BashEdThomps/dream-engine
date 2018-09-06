#include "DreamObject.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace Dream
{
    DreamObject::DreamObject(string classname) :_CLASSNAME_(classname) {}
    DreamObject::~DreamObject(){}

    DreamObject* DreamObject::getRawPtr()
    {
        return this;
    }

    weak_ptr<DreamObject>
    DreamObject::getWeakPtr()
    {
        return shared_from_this();
    }

    void DreamObject::setLogClassName(string name)
    {
       _CLASSNAME_ = name;

    }

    string DreamObject::getClassName()
    {
        return _CLASSNAME_;
    }

    std::shared_ptr<spdlog::logger> DreamObject::getLog() const
    {
        try
        {
            auto log = spdlog::get(_CLASSNAME_);
            if (log == nullptr)
            {
                log = spdlog::stdout_color_mt(_CLASSNAME_);
            }
            return log;
        }
        catch (spdlog::spdlog_ex ex)
        {
           cerr << "********** Exception getting log for " << _CLASSNAME_ <<" **********"
                << endl << ex.what() << endl;
           return nullptr;
        }
    }
}
