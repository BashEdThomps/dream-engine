#include "SharedAssetInstance.h"

#include "../Project/Project.h"
#include "../Project/ProjectRuntime.h"
#include "../Project/ProjectDirectory.h"

namespace Dream
{
    SharedAssetInstance::SharedAssetInstance
    (AssetDefinition* def, ProjectRuntime* runtime)
        : AssetInstance(def),
          mProjectRuntime(runtime)
    {
        setLogClassName("SharedAssetInstance");
    }

    SharedAssetInstance::~SharedAssetInstance()
    {

    }

    string SharedAssetInstance::getAssetFilePath
    (string fmt)
    {
        auto pDir = mProjectRuntime
                ->getProject()
                ->getDirectory();
        return pDir->getAssetAbsolutePath(mDefinition,fmt);

    }

    string SharedAssetInstance::getAssetDirectoryPath
    ()
    {
        auto pDir = mProjectRuntime
                ->getProject()
                ->getDirectory();
        return pDir->getAssetDirectoryPath(mDefinition);

    }
}
