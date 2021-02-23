#include "SharedAssetRuntime.h"

#include "Cache.h"
#include "AssetDefinition.h"
#include "Common/Logger.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"
#include "Storage/ProjectDirectory.h"
#include "Entity/EntityRuntime.h"

namespace octronic::dream
{
    SharedAssetRuntime::SharedAssetRuntime
    (ProjectRuntime* prt, AssetDefinition* def)
        : AssetRuntime(prt, def),
          mReloadFlag(false)
    {
    }

    SharedAssetRuntime::~SharedAssetRuntime()
    {

    }

    string SharedAssetRuntime::getAssetFilePath
    (const string& fmt)
    {
        auto pDir = mProjectRuntimeHandle->getProject()->getDirectory();
        return pDir->getAssetAbsolutePath(static_cast<AssetDefinition*>(mDefinitionHandle),fmt);
    }

    string SharedAssetRuntime::getAssetDirectoryPath
    ()
    {
        auto pDir = mProjectRuntimeHandle->getProject()->getDirectory();
        return pDir->getAssetDirectoryPath(static_cast<AssetDefinition*>(mDefinitionHandle));
    }

    void SharedAssetRuntime::addInstance(EntityRuntime* er)
    {
        auto itr = std::find(mInstances.begin(), mInstances.end(), er);
        if (itr == mInstances.end())
        {
            mInstances.push_back(er);
        }
    }

    void SharedAssetRuntime::removeInstance(EntityRuntime* er)
    {
        removeInstanceByUuid(er->getUuid());
    }

    void SharedAssetRuntime::removeInstanceByUuid(UuidType uuid)
    {
        auto itr = std::find_if(mInstances.begin(), mInstances.end(),
                    [&](EntityRuntime* runtime) {
                return runtime->getUuid() == uuid;
        });

        if (itr != mInstances.end())
        {
            mInstances.erase(itr);
        }
    }

    vector<EntityRuntime*>* SharedAssetRuntime::getInstanceVector()
    {
        return &mInstances;
    }

    bool SharedAssetRuntime::getReloadFlag() const
    {
        return mReloadFlag;
    }

    void SharedAssetRuntime::setReloadFlag(bool reloadFlag)
    {
        mReloadFlag = reloadFlag;
    }
}
