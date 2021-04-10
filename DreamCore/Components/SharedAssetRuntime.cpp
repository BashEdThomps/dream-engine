#include "SharedAssetRuntime.h"

#include "Cache.h"
#include "AssetDefinition.h"
#include "Common/Logger.h"

#include "Project/ProjectRuntime.h"
#include "Project/ProjectDirectory.h"
#include "Entity/EntityRuntime.h"

using std::dynamic_pointer_cast;

namespace octronic::dream
{
  SharedAssetRuntime::SharedAssetRuntime
  (ProjectRuntime& prt, AssetDefinition& def)
    : AssetRuntime(prt, def),
      mReloadFlag(false)
  {
  }

  void
  SharedAssetRuntime::addInstance
  (EntityRuntime& er)
  {
    auto itr = std::find_if(mInstances.begin(), mInstances.end(),
			[&](reference_wrapper<EntityRuntime> next)
			{ return next.get().getUuid() == er.getUuid(); });

    if (itr == mInstances.end())
    {
      mInstances.push_back(er);
    }
  }

  void
  SharedAssetRuntime::removeInstance
  (EntityRuntime& er)
  {
      removeInstanceByUuid(er.getUuid());
  }

  void
  SharedAssetRuntime::removeInstanceByUuid
  (UuidType uuid)
  {
    auto itr = find_if(mInstances.begin(), mInstances.end(),
                   [&](reference_wrapper<EntityRuntime>& runtime)
    {
        return runtime.get().getUuid() == uuid;
    });

    if (itr != mInstances.end()) mInstances.erase(itr);
  }

  vector<reference_wrapper<EntityRuntime>>
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
