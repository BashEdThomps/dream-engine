#include "Cache.h"

#include "Project/Project.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Project/ProjectRuntime.h"
#include "SharedAssetRuntime.h"
#include "AssetDefinition.h"



namespace octronic::dream
{
    Cache::Cache
    (const string& className, ProjectRuntime* parent)
        : LockableObject (className),
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
        if(dreamTryLock()){
            dreamLock();
            return mProjectRuntime
                    ->getProject()
                    ->getDirectory()
                    ->getAssetAbsolutePath(def);
        } dreamElseLockFailed
    }

    void Cache::removeRuntimeByUuid(UuidType uuid)
    {
        if(dreamTryLock()){
            dreamLock();

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
        } dreamElseLockFailed
    }

    void Cache::removeRuntime(AssetDefinition *def)
    {
        if(dreamTryLock()){
            dreamLock();
            if (def == nullptr)
            {
                return;
            }

            removeRuntimeByUuid(def->getUuid());
        } dreamElseLockFailed
    }

    void
    Cache::clear
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            for (auto* runtime : mRuntimes)
            {
                delete runtime;
            }
            mRuntimes.clear();
        } dreamElseLockFailed
    }

    AssetDefinition*
    Cache::getAssetDefinitionByUuid
    (UuidType uuid)
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mProjectRuntime->getAssetDefinitionByUuid(uuid);
        } dreamElseLockFailed
    }

    SharedAssetRuntime*
    Cache::getRuntime
    (AssetDefinition* def)
    {
        if(dreamTryLock())
        {
            dreamLock();
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
        } dreamElseLockFailed
    }

    SharedAssetRuntime*
    Cache::getRuntime
    (UuidType id)
    {
        if(dreamTryLock())
        {
            dreamLock();
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
        } dreamElseLockFailed
    }

    const vector<SharedAssetRuntime*>&
    Cache::getRuntimeVector
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mRuntimes;
        } dreamElseLockFailed
    }

    size_t Cache::runtimeCount()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mRuntimes.size();
        } dreamElseLockFailed
    }
}
