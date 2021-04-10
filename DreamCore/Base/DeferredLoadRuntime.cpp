#include "DeferredLoadRuntime.h"

using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
  // Loadable Runtime ========================================================

  DeferredLoadRuntime::DeferredLoadRuntime
  (ProjectRuntime& projectRuntime, Definition& def)
    : Runtime(def),
      mLoaded(false),
      mLoadError(false),
      mProjectRuntime(projectRuntime)
  {

    mLoadFromDefinitionTask = make_shared<RuntimeLoadFromDefinitionTask>(getProjectRuntime(), *this);
  }

  shared_ptr<RuntimeLoadFromDefinitionTask>
  DeferredLoadRuntime::getLoadFromDefinitionTask
  ()
  const
  {
    return mLoadFromDefinitionTask;
  }

  bool
  DeferredLoadRuntime::getLoaded
  ()
  const
  {
    return mLoaded;
  }

  void
  DeferredLoadRuntime::setLoaded
  (bool loaded)
  {
    mLoaded = loaded;
  }

  bool
  DeferredLoadRuntime::getLoadError
  ()
  const
  {
    return mLoadError;
  }

  void
  DeferredLoadRuntime::setLoadError
  (bool loadError)
  {
    mLoadError = loadError;
  }

  ProjectRuntime&
  DeferredLoadRuntime::getProjectRuntime
  ()
  const
  {
    return mProjectRuntime.get();
  }
}
