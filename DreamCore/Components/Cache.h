#pragma once

#include "../Common/LockableObject.h"
#include <vector>
#include <memory>

using std::vector;

namespace Dream
{
    class ProjectRuntime;
    class SharedAssetRuntime;
    class AssetDefinition;

    /**
     * @brief Cache is an abstract class that is used to define a common
     * interface for a instanciating and storing SharedAssetRuntime objects.
     * SharedAssetRuntime objects are used by more than one SceneObjectRuntime
     * or other AssetRuntime objects. These AssetRuntime objects are cached to
     * reduce the SceneRuntime memory footprint.
     */
    class Cache : public LockableObject
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
         * @return SharedAssetRuntime object specified by the given
         * AssetDefinition. This may be an existing object if one was previously
         * instanciated, a new object if one does not already exist, or nullptr
         * if the AssetDefinition was invalid.
         * @param definition The AssetDefinition for the AssetRuntime you wish
         * to instanciate.
         */
        SharedAssetRuntime* getRuntime(AssetDefinition* definition);

        /**
         * @return SharedAssetRuntime object specified by the given
         * AssetDefinition uuid. This may be an existing object if one was
         * previously instanciated, a new object if one does not already exist,
         * or nullptr if the uuid was invalid.
         * @param uuid definition The AssetDefinition for the AssetRuntime you wish
         * to instanciate.
         */
        SharedAssetRuntime* getRuntime(uint32_t uuid);

        /**
         * @return Reference to the vector of SharedAssetRuntimes managed by
         * this Cache.
         */
        const vector<SharedAssetRuntime*>& getRuntimeVector();

        /**
         * @return std::string of the absolute path to the AssetDefinition's
         * data directory within the Project's directory structure.
         * @param definition The AssetDefinition of which you want to get the
         * absolute path;
         */
        string getAbsolutePath(AssetDefinition* definition);

        /**
         * @brief Delete all AssetRuntimes managed by this Cache.
         */
        virtual void clear();
    protected:

        /**
         * @brief Internal method to instanciate a
         * SharedAssetRuntime that will be managed by this Cache.
         * @param definition AssetDefinition of Asset you wish to load
         * @return A SharedAssetRuntime or nullptr if unsuccesssful.
         */
        virtual SharedAssetRuntime* loadRuntime(AssetDefinition* definition) = 0;

        /**
         * @param uuid of the AssetDefinition to be retreived.
         * @return AssetDefinition specified by uuid, or nullptr if none is
         * found.
         */
        AssetDefinition* getAssetDefinitionByUuid(uint32_t uuid);

        /**
         * @brief Vector of SharedAssetRuntimes managed by this Cache.
         */
        vector<SharedAssetRuntime*> mRuntimes;

        /**
         * @brief Pointer to the ProjectRuntime that instanciated this Cache.
         */
        ProjectRuntime* mProjectRuntime;
    };
}
