#include "Cache.h"

#include "../Project/Project.h"
#include "../Project/ProjectDirectory.h"
#include "../Project/ProjectRuntime.h"
#include "SharedAssetInstance.h"
#include "AssetDefinition.h"

namespace Dream
{
    Cache::Cache
    (ProjectRuntime* parent)
        : DreamObject ("Cache"),
          mProjectRuntime(parent)
    {

    }

    Cache::~Cache
    ()
    {
       clear();
    }

    string
    Cache::getAbsolutePath
    (AssetDefinition* def)
    {
        return mProjectRuntime
                ->getProject()
                ->getDirectory()
                ->getAssetAbsolutePath(def);
    }

    void
    Cache::clear
    ()
    {
        for (auto* instance : mInstances)
        {
            delete instance;
        }
        mInstances.clear();
    }

    AssetDefinition*
    Cache::getAssetDefinitionByUuid
    (string uuid)
    {
        return mProjectRuntime->getAssetDefinitionByUuid(uuid);
    }

    SharedAssetInstance*
    Cache::getInstance
    (AssetDefinition* def)
    {
        if (def == nullptr)
        {
            return nullptr;
        }
       for (auto* instance : mInstances)
       {
           if (instance->getUuid().compare(def->getUuid()) == 0)
           {
               return instance;
           }
       }
       return loadInstance(def);
    }

    SharedAssetInstance*
    Cache::getInstance
    (string id)
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

    vector<SharedAssetInstance*>&
    Cache::getInstanceVector
    ()
    {
       return mInstances;
    }
}
