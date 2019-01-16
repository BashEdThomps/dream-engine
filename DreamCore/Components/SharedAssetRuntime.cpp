#include "SharedAssetRuntime.h"

#include "AssetDefinition.h"
#include "../Project/Project.h"
#include "../Project/ProjectRuntime.h"
#include "../Project/ProjectDirectory.h"

namespace Dream
{
    SharedAssetRuntime::SharedAssetRuntime
    (AssetDefinition* def, ProjectRuntime* runtime)
        : AssetRuntime(def),
          mProjectRuntime(runtime)
    {
        #ifdef DREAM_LOG
        setLogClassName("SharedAssetRuntime");
        #endif
    }

    SharedAssetRuntime::~SharedAssetRuntime()
    {

    }

    string SharedAssetRuntime::getAssetFilePath
    (string fmt)
    {
        auto pDir = mProjectRuntime
                ->getProject()
                ->getDirectory();
        return pDir->getAssetAbsolutePath(static_cast<AssetDefinition*>(mDefinition),fmt);

    }

    string SharedAssetRuntime::getAssetDirectoryPath
    ()
    {
        auto pDir = mProjectRuntime
                ->getProject()
                ->getDirectory();
        return pDir->getAssetDirectoryPath(static_cast<AssetDefinition*>(mDefinition));

    }
}
