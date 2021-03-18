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


namespace octronic::dream
{

  Directory::Directory(
      const weak_ptr<StorageManager>& fileManager,
      const string& dir)
    : mStorageManager(fileManager),
      mPath(dir)
  {
    LOG_TRACE("Directory: {} {}", __FUNCTION__, mPath);
  }

  Directory::~Directory
  ()
  {
    LOG_TRACE("Directory: {} {}", __FUNCTION__, mPath);
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
        abs << mPath << Constants::DIR_PATH_SEP << fileName;
        string absPath = abs.str();

        if (auto smLock = mStorageManager.lock())
        {
          if (auto subDir = smLock->openDirectory(absPath).lock())
          {
            if (!subDir->isDirectory())
            {
              smLock->closeDirectory(subDir);
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
            smLock->closeDirectory(subDir);
          }
        }
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
    auto nameStart = mPath.find_last_of(Constants::DIR_PATH_SEP);
    {
      if (nameStart != string::npos)
      {
        return mPath.substr(nameStart+1);
      }
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
#ifdef WIN32
    return mkdir(mPath.c_str()) == 0;
#else
    return mkdir(mPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#endif
  }

  bool
  Directory::deleteDirectory
  ()
  {
    LOG_DEBUG("Directory: Deleting directory {}",mPath);
    auto files = list();
    for (auto& file : files)
    {
      if (file.compare(".") == 0) continue;
      if (file.compare("..") == 0) continue;

      string absPath = mPath+Constants::DIR_PATH_SEP+file;
      if (auto smLock = mStorageManager.lock())
      {
        if (auto d = smLock->openDirectory(absPath).lock())
        {
          if (d->isDirectory())
          {
            d->deleteDirectory();
          }
          else
          {
            if (auto f = smLock->openFile(absPath).lock())
            {
              if (f->exists())
              {
                f->deleteFile();
              }
              smLock->closeFile(f);
            }
          }
          smLock->closeDirectory(d);
        }
      }
    }

    if (rmdir(mPath.c_str()) == 0)
    {
      return true;
    }
    LOG_ERROR("Directory: Unable to delete directory {}",mPath);
    return false;
  }

  weak_ptr<File>
  Directory::file
  (const string& fileName)
  {
    if (auto smLock = mStorageManager.lock())
    {
      stringstream ss;
      ss << mPath << Constants::DIR_PATH_SEP << fileName;
      return smLock->openFile(ss.str());
    }
    return weak_ptr<File>();
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
}
