#pragma once

#include "../Common/DreamObject.h"
#include <vector>

using namespace std;

namespace Dream
{
    class ProjectRuntime;
    class SharedAssetInstance;
    class AssetDefinition;

    class Cache : public DreamObject
    {
    public:
        Cache(ProjectRuntime* parent);
        virtual ~Cache();

        SharedAssetInstance* getInstance(AssetDefinition* definition);
        SharedAssetInstance* getInstance(string uuid);
        vector<SharedAssetInstance*>& getInstanceVector();
        string getAbsolutePath(AssetDefinition*);
        virtual void clear();
    protected:
        virtual SharedAssetInstance* loadInstance(AssetDefinition* def) = 0;
        AssetDefinition* getAssetDefinitionByUuid(string uuid);
        vector<SharedAssetInstance*> mInstances;
        ProjectRuntime* mProjectRuntime;
    };
}
