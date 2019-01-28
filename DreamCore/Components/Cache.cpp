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
        for (auto* Runtime : mRuntimes)
        {
            delete Runtime;
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
       for (auto* Runtime : mRuntimes)
       {
           if (Runtime->getUuid() == def->getUuid())
           {
               return Runtime;
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
       for (auto Runtime : mRuntimes)
       {
           if (Runtime->getUuid() == id)
           {
               return Runtime;
           }
       }
       return loadRuntime(getAssetDefinitionByUuid(id));
    }

    vector<SharedAssetRuntime*>&
    Cache::getRuntimeVector
    ()
    {
       return mRuntimes;
    }
}
