#include "Cache.h"

#include "../Project/Project.h"
#include "../Project/ProjectDirectory.h"
#include "../Project/ProjectRuntime.h"
#include "SharedAssetRuntime.h"
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
    (uint32_t uuid)
    {
        return mProjectRuntime->getAssetDefinitionByUuid(uuid);
    }

    SharedAssetRuntime*
    Cache::getInstance
    (AssetDefinition* def)
    {
        if (def == nullptr)
        {
            return nullptr;
        }
       for (auto* instance : mInstances)
       {
           if (instance->getUuid() == def->getUuid())
           {
               return instance;
           }
       }
       return loadInstance(def);
    }

    SharedAssetRuntime*
    Cache::getInstance
    (uint32_t id)
    {
       if (id == 0)
       {
            return nullptr;
       }
       for (auto instance : mInstances)
       {
           if (instance->getUuid() == id)
           {
               return instance;
           }
       }
       return loadInstance(getAssetDefinitionByUuid(id));
    }

    vector<SharedAssetRuntime*>&
    Cache::getInstanceVector
    ()
    {
       return mInstances;
    }
}
