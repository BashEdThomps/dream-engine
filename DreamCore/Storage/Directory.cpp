#include "Directory.h"

#include "StorageManager.h"
#include "File.h"
#include "Common/Logger.h"
#include "Common/Constants.h"

#ifdef WIN32
#include <dirent.h>
#include <direct.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <regex>
#include <sstream>

using std::regex;
using std::stringstream;
using std::cmatch;
using std::runtime_error;

namespace octronic::dream
{

  Directory::Directory(
      StorageManager& storageManager,
      const string& dir)
    : mStorageManager(storageManager),
      mPath(dir)
  {
    LOG_TRACE("Directory: Constructor {}", mPath);
  }

  vector<string>
  Directory::list
  (const string& regexStr)
  {
    LOG_TRACE("Directory: {}", __FUNCTION__);
    auto usingRegex = !regexStr.empty();
    vector<string> directoryContents;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(mPath.c_str())) != nullptr)
    {
      regex fileRegex(regexStr);
      cmatch match;

      while ((ent = readdir (dir)) != nullptr)
      {
        string fileName(ent->d_name);

        if (fileName.compare(".") == 0 || fileName.compare("..") == 0) continue;

        if (usingRegex)
        {
          if (regex_search(fileName.c_str(),match,fileRegex))
          {
            directoryContents.push_back(fileName);
          }
        }
        else
        {
          directoryContents.push_back(fileName);
        }
      }
      closedir (dir);
    }
    else
    {
      LOG_ERROR( "Directory: Unable to open directory {}", mPath );
      return directoryContents;
    }
    return directoryContents;
  }

  vector<string>
  Directory::listSubdirectories
  (const string& regexStr)
  {
    LOG_TRACE("Directory: {}", __FUNCTION__);
    auto usingRegex = !regexStr.empty();
    vector<string> directoryContents;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(mPath.c_str())) != nullptr)
    {
      regex fileRegex(regexStr);
      cmatch match;

      while ((ent = readdir (dir)) != nullptr)
      {
        string fileName(ent->d_name);
        if (fileName[0] == '.') continue;
        stringstream abs;
        abs << mPath << Constants::DIRECTORY_PATH_SEP << fileName;
        string absPath = abs.str();

        auto& sm = getStorageManager();
        auto& subDir = sm.openDirectory(absPath);
        if (!subDir.isDirectory())
        {
          sm.closeDirectory(subDir);
          continue;
        }

        if (usingRegex)
        {
          if (regex_search(fileName.c_str(),match,fileRegex))
          {
            directoryContents.push_back(absPath);
          }
        }
        else
        {
          directoryContents.push_back(absPath);
        }
        sm.closeDirectory(subDir);
      }
      closedir(dir);
    }
    else
    {
      LOG_ERROR( "Directory: Unable to open directory {}", mPath );
      return directoryContents;
    }
    return directoryContents;
  }

  string
  Directory::getPath
  () const
  {
    return mPath;
  }

  void
  Directory::setPath
  (const string& path)
  {
    mPath = path;
  }

  string
  Directory::getName
  ()
  const
  {
    auto nameStart = mPath.find_last_of(Constants::DIRECTORY_PATH_SEP);
    if (nameStart != string::npos)
    {
      return mPath.substr(nameStart+1);
    }
    return "";
  }

  bool
  Directory::exists
  ()
  const
  {
    DIR* dir = opendir(mPath.c_str());
    if (dir)
    {
      closedir(dir);
      return true;
    }
    return false;
  }

  bool
  Directory::create
  ()
  {
    if (mPath.size() == 0)
    {
      throw runtime_error("Directory: Cannot create directory, path is empty");
    }

#ifdef WIN32
    auto ret =  mkdir(mPath.c_str());
#else
    auto ret = mkdir(mPath.c_str(),
                     S_IRWXU | S_IRWXG | // Read/Write/Search for Group
                     S_IROTH | S_IXOTH);   // Read/Search for Other

    if (ret != 0)
    {
      stringstream ss;
      ss << "Error creating directory " ;
      ss << mPath;
      ss << "errno: ";
      ss << errno;
      throw runtime_error(ss.str());
    }
    return true;
#endif
  }

  bool
  Directory::deleteDirectory
  ()
  {
    LOG_DEBUG("Directory: Deleting directory {}",mPath);
    auto files = list();
    auto& sm = getStorageManager();
    for (auto& file : files)
    {
      if (file.compare(".") == 0) continue;
      if (file.compare("..") == 0) continue;

      stringstream absPath;
      absPath << mPath << Constants::DIRECTORY_PATH_SEP << file;
      auto& d = sm.openDirectory(absPath.str());
      if (d.isDirectory())
      {
        d.deleteDirectory();
      }
      else
      {
        auto& f = sm.openFile(absPath.str());
        if (f.exists())
        {
          f.deleteFile();
        }
        sm.closeFile(f);
      }
      sm.closeDirectory(d);
    }

    if (rmdir(mPath.c_str()) == 0)
    {
      return true;
    }
    LOG_ERROR("Directory: Unable to delete directory {}",mPath);
    return false;
  }

  File&
  Directory::file
  (const string& fileName)
  {
    stringstream ss;
    ss << mPath << Constants::DIRECTORY_PATH_SEP << fileName;
    return getStorageManager().openFile(ss.str());
  }

  bool
  Directory::isDirectory
  ()
  const
  {
    bool result = false;
    struct stat sb;
    if (stat(mPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
      result = true;
    }
    LOG_DEBUG("Directory: {} Directory {}",result? "Is a" : "Not a", mPath );
    return result;
  }


  StorageManager&
  Directory::getStorageManager
  ()
  const
  {
    return mStorageManager.get();
  }
}
