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
        : LockableObject ("Cache"),
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
        for (auto* runtime : mRuntimes)
        {
            delete runtime;
        }
        mRuntimes.clear();
    }

    AssetDefinition*
    Cache::getAssetDefinitionByUuid
    (uint32_t uuid)
    {
        return mProjectRuntime->getAssetDefinitionByUuid(uuid);
    }

    SharedAssetRuntime*
    Cache::getRuntime
    (AssetDefinition* def)
    {
        if (def == nullptr)
        {
            return nullptr;
        }
       for (auto* runtime : mRuntimes)
       {
           if (runtime->getUuid() == def->getUuid())
           {
               return runtime;
           }
       }
       return loadRuntime(def);
    }

    SharedAssetRuntime*
    Cache::getRuntime
    (uint32_t id)
    {
       if (id == 0)
       {
            return nullptr;
       }
       for (auto runtime : mRuntimes)
       {
           if (runtime->getUuid() == id)
           {
               return runtime;
           }
       }
       return loadRuntime(getAssetDefinitionByUuid(id));
    }

    const vector<SharedAssetRuntime*>&
    Cache::getRuntimeVector
    ()
    {
       return mRuntimes;
    }
}
