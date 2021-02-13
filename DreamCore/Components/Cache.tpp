#pragma once

#include "Project/ProjectRuntime.h"
#include "Project/Project.h"
#include "Storage/ProjectDirectory.h"

namespace octronic::dream
{
	template <typename DefinitionType, typename RuntimeType>
    Cache<DefinitionType, RuntimeType>::Cache
	(ProjectRuntime* parent)
        : mProjectRuntimeHandle(parent)
    {

    }

	template <typename DefinitionType, typename RuntimeType>
    Cache<DefinitionType, RuntimeType>::~Cache
    ()
    {
        clear();
    }

	template <typename DefinitionType, typename RuntimeType> string
	Cache<DefinitionType, RuntimeType>::getAbsolutePath
	(DefinitionType* def)
    {
        return mProjectRuntimeHandle->getProject()->getDirectory()->getAssetAbsolutePath(def);
    }

	template <typename DefinitionType, typename RuntimeType> void
	Cache<DefinitionType, RuntimeType>::removeRuntimeByUuid
	(UuidType uuid)
    {
        for (auto& runtime : mRuntimes)
        {
            if (runtime->getUuid() == uuid)
            {
                auto target_itr = find(mRuntimes.begin(), mRuntimes.end(), runtime);
				if (target_itr != mRuntimes.end())
				{
					mRuntimes.erase(target_itr);
                    return;
				}
            }
        }
    }

	template <typename DefinitionType, typename RuntimeType> void
    Cache<DefinitionType, RuntimeType>::removeRuntime
	(DefinitionType* def)
    {
        if (def == nullptr)
        {
            return;
        }

        removeRuntimeByUuid(def->getUuid());
    }

	template <typename DefinitionType, typename RuntimeType>
	void Cache<DefinitionType, RuntimeType>::clear
	()
    {
        mRuntimes.clear();
    }

	template <typename DefinitionType, typename RuntimeType> DefinitionType*
    Cache<DefinitionType, RuntimeType>::getAssetDefinitionByUuid
    (UuidType uuid)
    {
        return static_cast<DefinitionType*>(mProjectRuntimeHandle->getAssetDefinitionByUuid(uuid));
    }

	template <typename DefinitionType, typename RuntimeType> RuntimeType*
    Cache<DefinitionType, RuntimeType>::getRuntimeHandle
    (DefinitionType* def)
    {
        return getRuntimeHandle(def->getUuid());
    }

	template <typename DefinitionType, typename RuntimeType> RuntimeType*
    Cache<DefinitionType, RuntimeType>::getRuntimeHandle
    (UuidType id)
    {
        if (id == 0)
        {
            return nullptr;
        }

        for (auto& runtime : mRuntimes)
        {
            if (runtime->getUuid() == id)
            {
                return runtime.get();
            }
        }
        return loadRuntime(static_cast<DefinitionType*>(getAssetDefinitionByUuid(id)));

    }

	template <typename DefinitionType, typename RuntimeType> vector<unique_ptr<RuntimeType>>*
    Cache<DefinitionType, RuntimeType>::getRuntimeVectorHandle
    ()
    {
        return &mRuntimes;
    }

    template <typename DefinitionType, typename RuntimeType> size_t
	Cache<DefinitionType, RuntimeType>::runtimeCount
	()
    {
        return mRuntimes.size();
    }

	template <typename DefinitionType, typename RuntimeType> RuntimeType*
	Cache<DefinitionType, RuntimeType>::loadRuntime
	(DefinitionType* def)
    {
        if (!def)
        {
            LOG_ERROR("Cache: Cannot load Runtime. Definition is null");
            return nullptr;
        }

        RuntimeType* newRuntime = new RuntimeType(mProjectRuntimeHandle, def);
        mRuntimes.push_back(unique_ptr<RuntimeType>(newRuntime));
        LOG_TRACE("Cache: Pushed back new Runtime");
        return newRuntime;
	}
}


