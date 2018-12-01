#include "ProjectDirectory.h"

#include <sstream>

#include "../Utilities/Directory.h"
#include "../Utilities/File.h"
#include "../Project/Project.h"
#include "../Components/IAssetDefinition.h"
#include "ProjectDefinition.h"

using std::stringstream;

namespace Dream
{
    ProjectDirectory::ProjectDirectory
    (Project* proj)
        : DreamObject("ProjectDirectory"),
          mProject(proj)
    {

    }

    ProjectDirectory::~ProjectDirectory
    ()
    {
    }

    bool
    ProjectDirectory::baseDirectoryExists
    ()
    {
        Directory d(mProject->getProjectPath());
        return d.exists();
    }

    bool
    ProjectDirectory::createBaseDirectory
    ()
    {
        auto log = getLog();
        auto projPath = mProject->getProjectPath();
        log->debug("Creating project directory {}",projPath);
        Directory d(projPath);
        if(!d.create())
        {
            log->error("Unable to create project directory {}",projPath);
            return false;
        }
        return true;
    }

    bool
    ProjectDirectory::createAllAssetDirectories
    (string projPath)
    {
        auto log = getLog();
        auto assetTypes = Constants::DREAM_ASSET_TYPES_MAP;
        for (auto typePair : assetTypes)
        {
            auto type = typePair.first;
            if (!createAssetDirectory(projPath,type))
            {
                log->error("Unable to create asset directory {}",Constants::getAssetTypeStringFromTypeEnum(type));
                return false;
            }
        }
        return true;
    }

    bool
    ProjectDirectory::createAssetDirectory
    (string base, AssetType t)
    {
        auto log = getLog();
        stringstream ss;
        ss << base << Constants::DIR_PATH_SEP << Constants::getAssetTypeStringFromTypeEnum(t);
        auto dir = ss.str();
        log->debug("Creating asset directory {}",dir);
        Directory d(dir);
        return d.create();
    }

    vector<char>
    ProjectDirectory::readAssetData
    (IAssetDefinition* assetDef, string format)
    {
        auto path = getAssetAbsolutePath(assetDef,format);
        File f(path);
        return f.readBinary();
    }

    bool
    ProjectDirectory::writeAssetData
    (IAssetDefinition* assetDef, vector<char> data, string format)
    {
        auto log = getLog();
        auto dataPath = getAssetDirectoryPath(assetDef);
        log->debug("Writing asset data to {}",dataPath);

        //Check target directory exists
        Directory dir(dataPath);
        if (!dir.exists())
        {
            log->debug("Asset path does not exist {}",dataPath);
            auto assetTypeEnum = Constants::getAssetTypeEnumFromString(assetDef->getType());
            if (!assetTypeDirectoryExists(assetTypeEnum))
            {
                if(!createAssetTypeDirectory(assetTypeEnum))
                {
                    log->error("Unable to create asset type directory");
                    return false;
                }
            }
            if(!dir.create())
            {
                log->error("Unable to create asset path {}",dataPath);
                return false;
            }
        }
        auto path = getAssetAbsolutePath(assetDef,format);
        log->debug("Copying asset to {}",path);
        auto file = File(path);
        return file.writeBinary(data);
    }

    string
    ProjectDirectory::getAssetAbsolutePath
    (IAssetDefinition* assetDef, string format)
    {
        stringstream path;
        path << mProject->getProjectPath()
             << Constants::DIR_PATH_SEP
             << assetDef->getType()
             << Constants::DIR_PATH_SEP
             << assetDef->getUuid()
             << Constants::DIR_PATH_SEP
             << (format.empty() ? assetDef->getFormat() : format);
        return path.str();

    }

    string
    ProjectDirectory::getAssetDirectoryPath
    (IAssetDefinition* assetDef)
    {
        stringstream path;
        path << mProject->getProjectPath()
             << Constants::DIR_PATH_SEP
             << assetDef->getType()
             << Constants::DIR_PATH_SEP
             << assetDef->getUuid();
        return path.str();

    }

    bool
    ProjectDirectory::removeAssetDirectory
    (Dream::IAssetDefinition* ad)
    {
        auto log = getLog();
        auto path = getAssetDirectoryPath(ad);
        log->debug("Removing asset directory {}",path);
        Directory d(path);
        return d.deleteDirectory();
    }

    bool
    ProjectDirectory::saveProject
    ()
    {
        auto pDef = mProject->getProjectDefinition();
        if (pDef)
        {
            auto jsDef = pDef->getJson();
            auto jsonStr = jsDef.dump(1);
            auto path = getProjectFilePath();
            File f(path);
            return f.writeString(jsonStr);
        }
        return false;
    }

    string
    ProjectDirectory::getProjectFilePath
    ()
    {
        stringstream ss;
        ss << mProject->getProjectPath()
           << Constants::DIR_PATH_SEP
           << mProject->getProjectDefinition()->getUuid()
           << Constants::PROJECT_EXTENSION;
        return ss.str();
    }

    bool
    ProjectDirectory::assetTypeDirectoryExists
    (AssetType type)
    {
        string dirPath = getAssetTypeDirectory(type);
        Directory dir(dirPath);
        return dir.exists();
    }

    string
    ProjectDirectory::getAssetTypeDirectory
    (AssetType type)
    {
        stringstream ss;
        ss << mProject->getProjectPath()
           << Constants::DIR_PATH_SEP
           << Constants::getAssetTypeStringFromTypeEnum(type);
        return ss.str();
    }

    bool ProjectDirectory::createAssetTypeDirectory(AssetType type)
    {
        string assetTypeDirPath = getAssetTypeDirectory(type);
        Directory dir(assetTypeDirPath);
        return dir.create();
    }

    void
    ProjectDirectory::cleanupAssetsDirectory
    ()
    {
        auto log = getLog();
        auto pDef = mProject->getProjectDefinition();
        if (!pDef)
        {
            log->error("Cannot cleanup, no project definition");
            return;
        }

        for (auto typePair : Constants::DREAM_ASSET_TYPES_MAP)
        {
            AssetType type = typePair.first;
            string typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
            string path = getAssetTypeDirectory(type);
            Directory assetDir(path);
            if (assetDir.exists())
            {
                auto subdirs = assetDir.listSubdirectories();
                log->error("Cleaning up {} containing {} definitions", path, subdirs.size());
                int deletedCount=0;
                for (auto subdirPath : subdirs)
                {
                    Directory subdir(subdirPath);
                    if (subdir.exists())
                    {
                        auto name = subdir.getName();
                        log->error("Checking subdir {} has definition",name);
                        auto def = pDef->getAssetDefinitionByUuid(name);
                        if (!def)
                        {
                            log->error("Definition {} does not exist, removing...",name);
                            subdir.deleteDirectory();
                        }
                    }
                }
                log->error("Deleted {}/{} {} asset directories",deletedCount,subdirs.size(),typeStr);
            }
            else
            {
                log->error("No Directory {}",path);
            }
        }
    }

    const size_t ProjectDirectory::BufferSize = 1024*1024;
}
