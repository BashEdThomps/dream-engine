#include "SharedAssetRuntime.h"

#include "AssetDefinition.h"
#include "Common/Logger.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"
#include "Components/Storage/ProjectDirectory.h"

namespace octronic::dream
{
    SharedAssetRuntime::SharedAssetRuntime
    (AssetDefinition* def, ProjectRuntime* runtime)
        : AssetRuntime(def),
          mProjectRuntime(runtime)
    {
    }

    SharedAssetRuntime::~SharedAssetRuntime()
    {

    }

    string SharedAssetRuntime::getAssetFilePath
    (string fmt)
    {
        auto pDir = mProjectRuntime->getProject()->getDirectory();
        return pDir->getAssetAbsolutePath(static_cast<AssetDefinition*>(mDefinition),fmt);

    }

    string SharedAssetRuntime::getAssetDirectoryPath
    ()
    {
        auto pDir = mProjectRuntime->getProject()->getDirectory();
        return pDir->getAssetDirectoryPath(static_cast<AssetDefinition*>(mDefinition));
    }

    ProjectRuntime*
    SharedAssetRuntime::getProjectRuntime
    ()
    const
    {
        return mProjectRuntime;
    }
}
