#include "Cache.h"

#include "Project/Project.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Project/ProjectRuntime.h"
#include "SharedAssetRuntime.h"
#include "AssetDefinition.h"

namespace octronic::dream
{
    Cache::Cache
    (ProjectRuntime* parent)
        : LockableObject (),
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

    void Cache::removeRuntimeByUuid(UuidType uuid)
    {
        AssetRuntime* target = nullptr;

        for (auto* runtime : mRuntimes)
        {
            if (runtime->getUuid() == uuid)
            {
                target = runtime;
            }
        }

        if (target)
        {
            auto target_itr = find(mRuntimes.begin(), mRuntimes.end(), target);
            if (target_itr != mRuntimes.end())
            {
                mRuntimes.erase(target_itr);
            }
            delete target;
            target = nullptr;
        }
    }

    void Cache::removeRuntime(AssetDefinition *def)
    {
        if (def == nullptr)
        {
            return;
        }

        removeRuntimeByUuid(def->getUuid());
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
    (UuidType uuid)
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
    (UuidType id)
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

    size_t Cache::runtimeCount() const
    {
        return mRuntimes.size();
    }
}
