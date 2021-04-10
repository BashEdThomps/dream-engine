#pragma once


#include "Cache.h"

#include "Project/ProjectDefinition.h"
#include "Project/ProjectDirectory.h"
#include "Common/Logger.h"

#include <exception>

using std::make_unique;

namespace octronic::dream
{
  template <typename DefinitionType, typename RuntimeType>
  Cache<DefinitionType, RuntimeType>::Cache
  (ProjectRuntime& projRuntime,
   ProjectDefinition& projDef,
   ProjectDirectory& projDir)
    : mProjectRuntime(projRuntime),
      mProjectDefinition(projDef),
      mProjectDirectory(projDir)
  {}

  template <typename DefinitionType, typename RuntimeType>
  void
  Cache<DefinitionType, RuntimeType>::removeRuntimeByUuid
  (UuidType uuid)
  {
    std::remove_if(mRuntimes.begin(), mRuntimes.end(),
                   [&](const unique_ptr<RuntimeType>& nextRuntime)
    { return nextRuntime->getUuid() == uuid; });
  }

  template <typename DefinitionType, typename RuntimeType> void
  Cache<DefinitionType, RuntimeType>::removeRuntime
  (DefinitionType& def)
  {
    removeRuntimeByUuid(def.getUuid());
  }

  template <typename DefinitionType, typename RuntimeType>
  void
  Cache<DefinitionType, RuntimeType>::clear
  ()
  {
    mRuntimes.clear();
  }

  template <typename DefinitionType, typename RuntimeType>
  RuntimeType&
  Cache<DefinitionType, RuntimeType>::getRuntime
  (DefinitionType& def)
  {
    auto itr = std::find_if(mRuntimes.begin(), mRuntimes.end(),
                            [&](const unique_ptr<RuntimeType>& next)
    { return next->getUuid() == def.getUuid();});

    if (itr != mRuntimes.end()) return *(*itr);
    return loadRuntime(def);
  }

  template <typename DefinitionType, typename RuntimeType>
  vector<reference_wrapper<RuntimeType>>
  Cache<DefinitionType, RuntimeType>::getRuntimeVector
  ()
  const
  {
    vector<reference_wrapper<RuntimeType>> ret;
    for (auto& rt : mRuntimes)
    {
      ret.push_back(*rt);
    }
    return ret;
  }

  template <typename DefinitionType, typename RuntimeType>
  size_t
  Cache<DefinitionType, RuntimeType>::runtimeCount
  ()
  const
  {
    return mRuntimes.size();
  }

  template <typename DefinitionType, typename RuntimeType>
  RuntimeType&
  Cache<DefinitionType, RuntimeType>::loadRuntime
  (DefinitionType& def)
  {
    RuntimeType& newRuntime = *mRuntimes.emplace_back(make_unique<RuntimeType>(mProjectRuntime, def));
    LOG_TRACE("Cache: Pushed back new Runtime");
    return newRuntime;
  }

  template<typename DefinitionType, typename RuntimeType>
  ProjectRuntime&
  Cache<DefinitionType,RuntimeType>::getProjectRuntime
  ()
  const
  {
    return mProjectRuntime.get();
  }

  template<typename DefinitionType, typename RuntimeType>
  ProjectDirectory&
  Cache<DefinitionType,RuntimeType>::getProjectDirectory
  ()
  const
  {
    return mProjectDirectory.get();
  }

  template<typename DefinitionType, typename RuntimeType>
  ProjectDefinition&
  Cache<DefinitionType,RuntimeType>::getProjectDefinition
  ()
  const
  {
    return mProjectDefinition.get();
  }
}


