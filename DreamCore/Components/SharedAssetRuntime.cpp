#include "SharedAssetRuntime.h"

#include "Cache.h"
#include "AssetDefinition.h"
#include "Common/Logger.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"
#include "Storage/ProjectDirectory.h"
#include "Entity/EntityRuntime.h"

using std::dynamic_pointer_cast;

namespace octronic::dream
{
    SharedAssetRuntime::SharedAssetRuntime
    (const weak_ptr<ProjectRuntime>& prt,
     const weak_ptr<AssetDefinition>& def)
        : AssetRuntime(prt, def),
          mReloadFlag(false)
    {
    }

    SharedAssetRuntime::~SharedAssetRuntime
    ()
    {

    }

    void
    SharedAssetRuntime::addInstance
    (const weak_ptr<EntityRuntime>& er)
    {
        if (auto erLock = er.lock())
        {
			auto itr = std::find_if(mInstances.begin(), mInstances.end(),
			[&](const weak_ptr<EntityRuntime>& nextEr){ return nextEr.lock() == erLock; });
			if (itr == mInstances.end())
			{
				mInstances.push_back(er);
			}
        }
    }

    void
    SharedAssetRuntime::removeInstance
    (const weak_ptr<EntityRuntime>& er)
    {
        if (auto erLock = er.lock())
        {
        	removeInstanceByUuid(erLock->getUuid());
        }
    }

    void
    SharedAssetRuntime::removeInstanceByUuid
    (UuidType uuid)
    {
        auto itr = std::find_if(mInstances.begin(), mInstances.end(),
                    [&](const weak_ptr<EntityRuntime>& runtime)
        {
                return runtime.lock()->getUuid() == uuid;
        });

        if (itr != mInstances.end())
        {
            mInstances.erase(itr);
        }
    }

    vector<weak_ptr<EntityRuntime>>
    SharedAssetRuntime::getInstanceVector
    ()
    const
    {
        return mInstances;
    }

    bool
    SharedAssetRuntime::getReloadFlag
    ()
    const
    {
        return mReloadFlag;
    }

    void
    SharedAssetRuntime::setReloadFlag
    (bool reloadFlag)
    {
        mReloadFlag = reloadFlag;
    }
}
