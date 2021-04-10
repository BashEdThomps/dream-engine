#pragma once

#include <string>
#include <map>

using std::string;
using std::map;

namespace octronic::dream
{
    class Event
    {
    private:
        map<string,string> mAttributes;
        volatile bool mProcessed;

    public:
        Event(const map<string,string>& attr);
        ~Event();

        string getAttribute(const string& key) const;

        void setAttribute(const string& key, const string& value);
        void setProcessed(bool p);
        bool getProcessed() const;
    };
}
