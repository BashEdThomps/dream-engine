#include "SharedAssetRuntime.h"

#include "AssetDefinition.h"
#include "Common/Logger.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"
#include "Components/Storage/ProjectDirectory.h"

using std::unique_lock;

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        auto pDir = mProjectRuntime->getProject()->getDirectory();
        return pDir->getAssetAbsolutePath(static_cast<AssetDefinition*>(mDefinition),fmt);

    }

    string SharedAssetRuntime::getAssetDirectoryPath
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        auto pDir = mProjectRuntime->getProject()->getDirectory();
        return pDir->getAssetDirectoryPath(static_cast<AssetDefinition*>(mDefinition));
    }

    ProjectRuntime*
    SharedAssetRuntime::getProjectRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mProjectRuntime;
    }
}
