#pragma once


#include "Cache.h"

#include "Project/ProjectDefinition.h"
#include "Project/ProjectDirectory.h"
#include "Common/Logger.h"

#include <exception>


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
    	[&](const RuntimeType& nextRuntime) { return nextRuntime.getUuid() == uuid; });
    throw std::exception();
  }

  template <typename DefinitionType, typename RuntimeType> void
  Cache<DefinitionType, RuntimeType>::removeRuntime
  (DefinitionType& def)
  {
    removeRuntimeByUuid(def.getUuid());
    return;
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
    	[&](const RuntimeType& next) { return next.getUuid() == def.getUuid();});
    if (itr != mRuntimes.end()) return (*itr);
    return loadRuntime(def);
  }

  template <typename DefinitionType, typename RuntimeType>
  vector<RuntimeType>&
  Cache<DefinitionType, RuntimeType>::getRuntimeVector
  ()
  {
    return mRuntimes;
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
    RuntimeType& newRuntime = mRuntimes.emplace_back(mProjectRuntime, def);
    if (newRuntime.init())
    {
      LOG_TRACE("Cache: Pushed back new Runtime");
      return newRuntime;
    }
    else
    {
      removeRuntime(def);
    	throw std::exception();
    }
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


