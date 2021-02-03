#include "Definition.h"

namespace octronic::dream
{
    Definition::Definition(const string& className, const json& data) : LockableObject(className), mJson(data)
    {
        if (mJson[Constants::UUID].is_string())
        {
            mJson[Constants::UUID] = Uuid::generateUuid();
        }
    }

    Definition::~Definition() {}

    json Definition::getJson
    ()
    {
        return mJson;
    }

    bool Definition::hasName(const string& name)
    {
        string s = mJson[Constants::NAME];
        return s.compare(name) == 0;
    }

    string Definition::getName()
    {
        if (mJson[Constants::NAME].is_null())
        {
            mJson[Constants::NAME] = "";
        }
        return mJson[Constants::NAME];
    }

    void Definition::setName(const string& name)
    {
        mJson[Constants::NAME] = name;
    }

    bool Definition::hasUuid(UuidType uuid)
    {
        UuidType s = mJson[Constants::UUID];
        return s == uuid;
    }

    UuidType Definition::getUuid()
    {
        if (mJson[Constants::UUID].is_null())
        {
            mJson[Constants::UUID] = Uuid::generateUuid();
        }
        return mJson[Constants::UUID];
    }

    void Definition::setUuid(UuidType uuid)
    {
        mJson[Constants::UUID] = uuid;
    }

    string Definition::getNameAndUuidString()
    {
        stringstream ss;
        ss << "[" << getName() << " : " << getUuid() << "]";
        return ss.str();
    }

    json Definition::wrapVector3(const Vector3& v)
    {
        json retval = json::object();
        retval[Constants::X] = v.x();
        retval[Constants::Y] = v.y();
        retval[Constants::Z] = v.z();
        return retval;
    }

    Vector3 Definition::unwrapVector3(const json& j)
    {
        return Vector3
        (
            j[Constants::X],
            j[Constants::Y],
            j[Constants::Z]
        );
    }
}