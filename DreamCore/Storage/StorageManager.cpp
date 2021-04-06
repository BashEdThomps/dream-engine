#include "StorageManager.h"

#include "File.h"
#include "Directory.h"
#include "Common/Logger.h"

#include <sstream>
#include <memory>

using std::stringstream;
using std::runtime_error;

namespace octronic::dream
{
  StorageManager::StorageManager
  ()
  {
    LOG_TRACE("StorageManager: {}", __FUNCTION__);
  }

  File&
  StorageManager::openFile
  (const string& file_path)
  {
    if (file_path.empty()) throw runtime_error("StorageManager: Cannot open File with empty path");

    LOG_TRACE("StorageManager: {} {}", __FUNCTION__, file_path);

    auto file_itr = std::find_if(mOpenFiles.begin(),mOpenFiles.end(),
                                 [&](const File& next_file)
    { return next_file.getPath().compare(file_path) == 0; });

    if (file_itr != mOpenFiles.end())
    {
      (*file_itr).incrementUseCount();
      return (*file_itr);
    }
    auto& ret = mOpenFiles.emplace_back(file_path);
    ret.incrementUseCount();
    return ret;
  }

  void
  StorageManager::closeFile
  (const File& file)
  {
    LOG_TRACE("StorageManager: {} {}", __FUNCTION__, file.getPath());

    auto file_itr = std::find_if(mOpenFiles.begin(), mOpenFiles.end(),
                                 [&](const File& next_file)
    { return next_file.getPath().compare(file.getPath()) == 0; });

    if (file_itr != mOpenFiles.end())
    {
      (*file_itr).decrementUseCount();
      if ((*file_itr).hasNoUsers())
      {
        mOpenFiles.erase(file_itr);
        LOG_DEBUG("StorageManager: Closed File {}",file.getPath());
      }
    }
    else
    {
      LOG_ERROR("StorageManager: FATAL - Attempted to close File not opened by StorageManager");
      stringstream ss;
      ss << "Attempted to close unmanaged file: \"" << file.getPath() << "\"";
      throw std::runtime_error(ss.str());
    }
  }

  Directory&
  StorageManager::openDirectory
  (const string& path)
  {
    if (path.size() == 0) throw runtime_error("StorageManager: Cannot open Directory with empty path");

    LOG_TRACE("StorageManager: {} {}", __FUNCTION__, path);

    auto dir_itr = find_if(mOpenDirectories.begin(), mOpenDirectories.end(),
          [&](const Directory& next_dir)
    { return next_dir.getPath().compare(path) == 0; });

    if (dir_itr != mOpenDirectories.end())
    {
      (*dir_itr).incrementUseCount();
      return (*dir_itr);
    }

    auto& ret = mOpenDirectories.emplace_back(*this, path);
    ret.incrementUseCount();
    return ret;
  }

  void
  StorageManager::closeDirectory
  (const Directory& d)
  {
    LOG_TRACE("StorageManager: {} {}", __FUNCTION__, d.getPath());
    auto dir_itr = std::find_if( mOpenDirectories.begin(), mOpenDirectories.end(),
                                 [&](const Directory& next_dir)
    { return next_dir.getPath().compare(d.getPath()) == 0; });

    if (dir_itr != mOpenDirectories.end())
    {
      (*dir_itr).decrementUseCount();
      if ((*dir_itr).hasNoUsers())
      {
        mOpenDirectories.erase(dir_itr);
        LOG_DEBUG("StorageManager: Closed Directory {}",d.getPath());
      }
    }
    else
    {
      LOG_ERROR("StorageManager: FATAL - Attempted to close Directory not managed by StorageManager");
      stringstream ss;
      ss << "Attempted to close unmanaged Directory: \"" << d.getPath() << "\"";
      throw std::runtime_error(ss.str());
    }
  }
}
