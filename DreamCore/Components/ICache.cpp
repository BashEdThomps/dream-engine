#include "ICache.h"
#include "../Project/ProjectRuntime.h"

namespace Dream
{
    ICache::ICache
    (ProjectRuntime* parent)
        : DreamObject ("ICache"),
          mProjectRuntime(parent)
    {

    }

    ICache::~ICache
    ()
    {
       clear();
    }

    std::string
    ICache::getAbsolutePath
    (IAssetDefinition* def)
    {
        return mProjectRuntime->getAssetAbsolutePath(def->getUuid());
    }

    void
    ICache::clear
    ()
    {
        for (IAssetInstance* instance : mInstances)
        {
            delete instance;
        }
        mInstances.clear();
    }

    IAssetDefinition*
    ICache::getAssetDefinitionByUuid
    (string uuid)
    {
        return mProjectRuntime->getAssetDefinitionByUuid(uuid);
    }

    IAssetInstance*
    ICache::getInstance
    (IAssetDefinition* def)
    {
        if (def == nullptr)
        {
            return nullptr;
        }
       for (auto instance : mInstances)
       {
           if (instance->getUuid().compare(def->getUuid()) == 0)
           {
               return instance;
           }
       }
       return loadInstance(def);
    }

    IAssetInstance*
    ICache::getInstance
    (std::string id)
    {
       if (id.empty())
       {
            return nullptr;
       }
       for (auto instance : mInstances)
       {
           if (instance->getUuid().compare(id) == 0)
           {
               return instance;
           }
       }
       return loadInstance(getAssetDefinitionByUuid(id));
    }
}
