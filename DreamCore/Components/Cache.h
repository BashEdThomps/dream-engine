#pragma once

#include "../Common/DreamObject.h"
#include <vector>

using namespace std;

namespace Dream
{
    class ProjectRuntime;
    class SharedAssetRuntime;
    class AssetDefinition;

    /**
     * @brief Cache is an abstract class that is used to define a common
     * interface for a instanciating and storing SharedAssetInstance objects.
     * SharedAssetInstance objects are used by more than one SceneObjectRuntime
     * or other AssetInstance objects. These AssetInstance objects are cached to
     * reduce the SceneRuntime memory footprint.
     */
    class Cache : public DreamObject
    {
    public:

        /**
         * @brief Default Constructor.
         * @param parent ProjectRuntime that created this Cache.
         */
        Cache(ProjectRuntime* parent);

        /**
         * @brief Default Destructor
         */
        virtual ~Cache();

        /**
         * @return SharedAssetInstance object specified by the given
         * AssetDefinition. This may be an existing object if one was previously
         * instanciated, a new object if one does not already exist, or nullptr
         * if the AssetDefinition was invalid.
         * @param definition The AssetDefinition for the AssetInstance you wish
         * to instanciate.
         */
        SharedAssetRuntime* getInstance(AssetDefinition* definition);

        /**
         * @return SharedAssetInstance object specified by the given
         * AssetDefinition uuid. This may be an existing object if one was
         * previously instanciated, a new object if one does not already exist,
         * or nullptr if the uuid was invalid.
         * @param uuid definition The AssetDefinition for the AssetInstance you wish
         * to instanciate.
         */
        SharedAssetRuntime* getInstance(string uuid);

        /**
         * @return Reference to the vector of SharedAssetInstances managed by
         * this Cache.
         */
        vector<SharedAssetRuntime*>& getInstanceVector();

        /**
         * @return std::string of the absolute path to the AssetDefinition's
         * data directory within the Project's directory structure.
         * @param definition The AssetDefinition of which you want to get the
         * absolute path;
         */
        string getAbsolutePath(AssetDefinition* definition);

        /**
         * @brief Delete all AssetInstances managed by this Cache.
         */
        virtual void clear();
    protected:

        /**
         * @brief Internal method to instanciate a
         * SharedAssetInstance that will be managed by this Cache.
         * @param definition AssetDefinition of Asset you wish to load
         * @return A SharedAssetInstance or nullptr if unsuccesssful.
         */
        virtual SharedAssetRuntime* loadInstance(AssetDefinition* definition) = 0;

        /**
         * @param uuid of the AssetDefinition to be retreived.
         * @return AssetDefinition specified by uuid, or nullptr if none is
         * found.
         */
        AssetDefinition* getAssetDefinitionByUuid(string uuid);

        /**
         * @brief Vector of SharedAssetInstances managed by this Cache.
         */
        vector<SharedAssetRuntime*> mInstances;

        /**
         * @brief Pointer to the ProjectRuntime that instanciated this Cache.
         */
        ProjectRuntime* mProjectRuntime;
    };
}
