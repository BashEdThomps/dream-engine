#include "Definition.h"

namespace octronic::dream
{
    // public
    Definition::Definition(const string& className, const json& data)
        : mJson(data)
    {
        if (mJson[Constants::UUID].is_string())
        {
            mJson[Constants::UUID] = Uuid::generateUuid();
        }
    }

    // public
    Definition::~Definition()
    {}

    // public
    json Definition::toJson
    ()
    {
        return mJson;
    }

    bool // public
    Definition::hasName(const string& name)
    const
    {
        string s = mJson[Constants::NAME];
        return s.compare(name) == 0;
    }

    string  // public
    Definition::getName()
    const
    {
        if (mJson.find(Constants::NAME) == mJson.end())
        {
            return "";
        }
        return mJson[Constants::NAME];
    }

    void // public
    Definition::setName(const string& name)
    {
        mJson[Constants::NAME] = name;
    }

    bool // public
    Definition::hasUuid(UuidType uuid)
    const
    {
        UuidType s = mJson[Constants::UUID];
        return s == uuid;
    }

    UuidType // public
    Definition::getUuid()
    const
    {
        if (mJson.find(Constants::UUID) == mJson.end())
        {
            return Uuid::INVALID;
        }
        return mJson[Constants::UUID];
    }

    void  // public
    Definition::setUuid(UuidType uuid)
    {
        mJson[Constants::UUID] = uuid;
    }

    string  // public
    Definition::getNameAndUuidString()
    const
    {
        stringstream ss;
        ss << "[" << getName() << " : " << getUuid() << "]";
        return ss.str();
    }
}
