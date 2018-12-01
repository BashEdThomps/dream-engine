#include "Directory.h"
#include "File.h"

#ifdef WIN32
#include "../deps/dirent.h"
#include <direct.h>
#else
#include <dirent.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <regex>
#include <sstream>

using std::regex;

namespace Dream
{

    Directory::Directory(string dir):
        DreamObject("Directory"),
        mPath(dir)
    {

    }

    Directory::~Directory()
    {

    }

    vector<string>
    Directory::list
    (string regexStr)
    {
        auto log=getLog();
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
            log->error( "Unable to open directory {}", mPath );
            return directoryContents;
        }
        return directoryContents;
    }

    vector<string>
    Directory::listSubdirectories
    (string regexStr)
    {
        auto log=getLog();
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
                const char* absPath = abs.str().c_str();

                Directory subDir(absPath);
                if (!subDir.isDirectory())
                {
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
            }
            closedir (dir);
        }
        else
        {
            log->error( "Unable to open directory {}", mPath );
            return directoryContents;
        }
        return directoryContents;

    }

    string Directory::getPath() const
    {
        return mPath;
    }

    void
    Directory::setPath
    (string path)
    {
        mPath = path;
    }

    string Directory::getName()
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

    bool Directory::deleteDirectory()
    {
        auto log = getLog();
        log->debug("Deleting directory {}",mPath);
        auto files = list();
        for (auto file : files)
        {
            if (file.compare(".") == 0) continue;
            if (file.compare("..") == 0) continue;

            string absPath = mPath+Constants::DIR_PATH_SEP+file;
            Directory d(absPath);
            if (d.isDirectory())
            {
                d.deleteDirectory();
            }
            else
            {
                File f(absPath);
                if (f.exists())
                {
                    f.deleteFile();
                }
            }
        }
        if (rmdir(mPath.c_str()) != 0)
        {
            log->error("Unable to delete directory {}",mPath);
            return false;
        }
        return true;
    }

    File
    Directory::file
    (string fileName)
    {
        stringstream ss;
        ss << mPath << Constants::DIR_PATH_SEP << fileName;
        return File(ss.str());
    }

    bool Directory::isDirectory()
    {
        auto log = getLog();
        bool result = false;
        struct stat sb;
        if (stat(mPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        {
            result = true;
        }
        log->error
        (
           "{} Directory {}",
           result? "Is a" : "Not a",
           mPath
        );
        return result;
    }
}
