#include "DiscreteAssetRuntime.h"

#include "AssetDefinition.h"

#include "Cache.h"
#include "Common/Logger.h"
#include "Project/ProjectRuntime.h"
#include "Project/ProjectDirectory.h"
#include "Entity/EntityRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Task/Task.h"

namespace octronic::dream
{
  DiscreteAssetRuntime::DiscreteAssetRuntime
  (ProjectRuntime& prt, AssetDefinition& def, EntityRuntime& runtime)
    : AssetRuntime (prt, def),
      mEntityRuntime(runtime)
  {
  }

  EntityRuntime&
  DiscreteAssetRuntime::getEntityRuntime
  ()
  const
  {
    return mEntityRuntime.get();
  }
}
