#include "DeferredLoadRuntime.h"

using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
    // Loadable Runtime ========================================================

    DeferredLoadRuntime::DeferredLoadRuntime
    (const weak_ptr<ProjectRuntime>& projectRuntime,
     const weak_ptr<Definition>& def)
        : Runtime(def),
          mProjectRuntime(projectRuntime),
          mLoaded(false),
          mLoadError(false)
    {

    }

    bool
    DeferredLoadRuntime::init
    ()
    {
        mLoadFromDefinitionTask =
        	make_shared<RuntimeLoadFromDefinitionTask>(
            	mProjectRuntime,
                static_pointer_cast<DeferredLoadRuntime>(shared_from_this()));
        return true;
    }

    DeferredLoadRuntime::~DeferredLoadRuntime()
    {

    }

    weak_ptr<RuntimeLoadFromDefinitionTask>
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

    weak_ptr<ProjectRuntime>
    DeferredLoadRuntime::getProjectRuntime
    ()
    const
    {
        return mProjectRuntime;
    }
}
