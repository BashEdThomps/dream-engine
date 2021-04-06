#pragma once

#include "Common/Uuid.h"
#include "Common/AssetType.h"

#include <string>
#include <vector>
#include <memory>

using std::vector;
using std::string;
using std::reference_wrapper;

namespace octronic::dream
{
  class ProjectRuntime;
  class ProjectDefinition;
  class ProjectDirectory;
  class SharedAssetRuntime;
  class AssetDefinition;

  /**
     * @brief Cache is an abstract class that is used to define a common
     * interface for a instanciating and storing SharedAssetRuntime objects.
     *
     * SharedAssetRuntime objects are used by more than one EntityRuntime
     * or other AssetRuntime objects. These AssetRuntime objects are cached to
     * reduce the SceneRuntime memory footprint.
     */
  template <typename DefinitionType, typename RuntimeType>
  class Cache final
  {
  public:
    /**
         * @brief Default Constructor.
         * @param parent ProjectRuntime that created this Cache.
         */
    Cache(ProjectRuntime& projRuntime, ProjectDefinition& projDef, ProjectDirectory& projDir);

    Cache(Cache&&) = default;
    Cache& operator=(Cache&&) = default;

    Cache(const Cache&) = delete;
    Cache& operator=(const Cache&) = delete;

    /**
         * @return SharedAssetRuntime object specified by the given
         * AssetDefinition. This may be an existing object if one was previously
         * instanciated, a new object if one does not already exist, or nullptr
         * if the AssetDefinition was invalid.
         * @param definition The AssetDefinition for the AssetRuntime you wish
         * to instanciate.
         */
    RuntimeType& getRuntime(DefinitionType& definition);

    /**
         * @return Reference to the vector of SharedAssetRuntimes managed by
         * this Cache.
         */
    vector<RuntimeType>& getRuntimeVector();

    /**
         * @brief removeRuntime remove a runtime from the cache based on definition
         * @param definition
         */
    void removeRuntime(DefinitionType& definition);

    /**
         * @brief removeRuntime remove a runtime from the cache based on uuid
         * @param definition
         */
    void removeRuntimeByUuid(UuidType uuid);

    /**
     * @brief Delete all AssetRuntimes managed by this Cache.
     */
    void clear();

    /**
     * @brief runtimeCount
     * @return the number of Runtimes in this Cache
     */
    size_t runtimeCount() const;

  protected:
    ProjectRuntime& getProjectRuntime() const;
    ProjectDirectory& getProjectDirectory() const;
    ProjectDefinition& getProjectDefinition() const;

    /**
     * @brief Internal method to instanciate a
     * SharedAssetRuntime that will be managed by this Cache.
     * @param definition AssetDefinition of Asset you wish to load
     * @return A SharedAssetRuntime or nullptr if unsuccesssful.
     *
     * LoadRuntime should not implement any logic, only call logic used
     * to load an Asset's data into RAM.
     */
    RuntimeType& loadRuntime(DefinitionType& definition);

    /**
     * @brief Vector of SharedAssetRuntimes managed by this Cache.
     */
    vector<RuntimeType> mRuntimes;

    /**
     * @brief Pointer to the ProjectRuntime that instanciated this Cache.
     */
    reference_wrapper<ProjectRuntime> mProjectRuntime;
    reference_wrapper<ProjectDirectory> mProjectDirectory;
    reference_wrapper<ProjectDefinition> mProjectDefinition;
  };
}

#include "Cache.tpp"
