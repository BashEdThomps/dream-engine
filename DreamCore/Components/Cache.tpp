#pragma once

#include "Project/ProjectRuntime.h"
#include "Project/Project.h"
#include "Storage/ProjectDirectory.h"

using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
  template <typename DefinitionType, typename RuntimeType>
  Cache<DefinitionType, RuntimeType>::Cache
  (const weak_ptr<ProjectRuntime>& parent)
    : mProjectRuntime(parent)
  {

  }

  template <typename DefinitionType, typename RuntimeType>
  Cache<DefinitionType, RuntimeType>::~Cache
  ()
  {
  }

  template <typename DefinitionType, typename RuntimeType>
  string
  Cache<DefinitionType, RuntimeType>::getAbsolutePath
  (const weak_ptr<DefinitionType>& def)
  const
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto dirLock = prLock->getPojectDirectory().lock())
      {
        return dirLock->getAssetAbsolutePath(def);
      }
    }
    return weak_ptr<RuntimeType>();
  }

  template <typename DefinitionType, typename RuntimeType>
  void
  Cache<DefinitionType, RuntimeType>::removeRuntimeByUuid
  (UuidType uuid)
  {
    for (auto& runtime : mRuntimes)
    {
      if (runtime->getUuid() == uuid)
      {
        auto target_itr = find(mRuntimes.begin(), mRuntimes.end(), runtime);
        if (target_itr != mRuntimes.end())
        {
          mRuntimes.erase(target_itr);
          return;
        }
      }
    }
  }

  template <typename DefinitionType, typename RuntimeType> void
  Cache<DefinitionType, RuntimeType>::removeRuntime
  (const weak_ptr<DefinitionType>& def)
  {
    if (def.expired())
    {
      return;
    }

    removeRuntimeByUuid(def->getUuid());
  }

  template <typename DefinitionType, typename RuntimeType>
  void
  Cache<DefinitionType, RuntimeType>::clear
  ()
  {
    mRuntimes.clear();
  }

  template <typename DefinitionType, typename RuntimeType>
  weak_ptr<DefinitionType>
  Cache<DefinitionType, RuntimeType>::getAssetDefinitionByUuid
  (UuidType uuid)
  {
    if(auto prLock = mProjectRuntime.lock())
    {
      if (auto def = prLock->getAssetDefinitionByUuid(uuid).lock())
      {
        return static_pointer_cast<DefinitionType>(def);
      }
    }
    return weak_ptr<DefinitionType>();
  }

  template <typename DefinitionType, typename RuntimeType>
  weak_ptr<RuntimeType>
  Cache<DefinitionType, RuntimeType>::getRuntime
  (const weak_ptr<DefinitionType>& def)
  {
    if (auto defLock = def.lock())
    {
      for (auto runtime : mRuntimes)
      {
        if (runtime->getUuid() == defLock->getUuid())
        {
          return runtime;
        }
      }
    }
    return loadRuntime(def);
  }

  template <typename DefinitionType, typename RuntimeType>
  weak_ptr<RuntimeType>
  Cache<DefinitionType, RuntimeType>::getRuntime
  (UuidType id)
  {
    if (id != Uuid::INVALID)
    {
      if (auto def = getAssetDefinitionByUuid(id).lock())
      {
        return getRuntime(static_pointer_cast<DefinitionType>(def));
      }
    }
    return weak_ptr<RuntimeType>();
  }

  template <typename DefinitionType, typename RuntimeType>
  vector<weak_ptr<RuntimeType>>
  Cache<DefinitionType, RuntimeType>::getRuntimeVector
  ()
  const
  {
    vector<weak_ptr<RuntimeType>> v;
    v.insert(v.begin(), mRuntimes.begin(), mRuntimes.end());
    return v;
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
  weak_ptr<RuntimeType>
  Cache<DefinitionType, RuntimeType>::loadRuntime
  (const weak_ptr<DefinitionType>& def)
  {
    auto newRuntime = make_shared<RuntimeType>(mProjectRuntime, def);

    if (newRuntime->init())
    {
      mRuntimes.push_back(newRuntime);
      LOG_TRACE("Cache: Pushed back new Runtime");
      return newRuntime;
    }

    return weak_ptr<RuntimeType>();
  }
}


