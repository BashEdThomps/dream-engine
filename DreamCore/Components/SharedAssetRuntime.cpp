#include "SharedAssetRuntime.h"

#include "AssetDefinition.h"
#include "Common/Logger.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"
#include "Components/Storage/ProjectDirectory.h"



namespace octronic::dream
{
    SharedAssetRuntime::SharedAssetRuntime
    (const string& className, AssetDefinition* def, ProjectRuntime* runtime)
        : AssetRuntime(className, def),
          mProjectRuntime(runtime)
    {
    }

    SharedAssetRuntime::~SharedAssetRuntime()
    {

    }

    string SharedAssetRuntime::getAssetFilePath
    (const string& fmt)
    {
        if(dreamTryLock())
        {
            dreamLock();
            auto pDir = mProjectRuntime->getProject()->getDirectory();
            return pDir->getAssetAbsolutePath(static_cast<AssetDefinition*>(mDefinition),fmt);
        } dreamElseLockFailed

    }

    string SharedAssetRuntime::getAssetDirectoryPath
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            auto pDir = mProjectRuntime->getProject()->getDirectory();
            return pDir->getAssetDirectoryPath(static_cast<AssetDefinition*>(mDefinition));
        } dreamElseLockFailed
    }

    ProjectRuntime*
    SharedAssetRuntime::getProjectRuntime
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mProjectRuntime;
        } dreamElseLockFailed
    }
}
