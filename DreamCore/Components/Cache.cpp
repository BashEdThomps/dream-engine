#include "Cache.h"

#include "Project/Project.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Project/ProjectRuntime.h"
#include "SharedAssetRuntime.h"
#include "AssetDefinition.h"

using std::unique_lock;

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        clear();
    }

    string
    Cache::getAbsolutePath
    (AssetDefinition* def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mProjectRuntime
                ->getProject()
                ->getDirectory()
                ->getAssetAbsolutePath(def);
    }

    void Cache::removeRuntimeByUuid(UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mProjectRuntime->getAssetDefinitionByUuid(uuid);
    }

    SharedAssetRuntime*
    Cache::getRuntime
    (AssetDefinition* def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mRuntimes;
    }

    size_t Cache::runtimeCount()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mRuntimes.size();
    }
}
