#include "AssetRuntime.h"

#include "AssetDefinition.h"
#include "Project/ProjectRuntime.h"
#include "Project/ProjectDirectory.h"
#include "Common/Uuid.h"

namespace octronic::dream
{
  AssetRuntime::AssetRuntime
  (ProjectRuntime& pr,
   AssetDefinition& definition)
    : DeferredLoadRuntime(pr, definition)

  {
  }


  AssetRuntime::~AssetRuntime
  ()
  {
  }
}


