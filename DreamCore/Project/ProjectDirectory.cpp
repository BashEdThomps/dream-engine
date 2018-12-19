#include "ProjectDirectory.h"

#include <sstream>

#include "../Utilities/Directory.h"
#include "../Utilities/File.h"
#include "../Project/Project.h"
#include "../Components/AssetDefinition.h"
#include "ProjectDefinition.h"

using std::stringstream;

namespace Dream
{
    ProjectDirectory::ProjectDirectory()
        : DreamObject ("ProjectDirectory"),
         mProject(nullptr)
    {

    }

    ProjectDirectory::ProjectDirectory
    (Project* proj)
        : DreamObject("ProjectDirectory"),
          mProject(proj)
    {

    }

    ProjectDirectory::~ProjectDirectory
    ()
    {
        closeProject();

    }

    bool
    ProjectDirectory::baseDirectoryExists
    ()
    {
        Directory d(mPath);
        return d.exists();
    }

    bool
    ProjectDirectory::createBaseDirectory
    ()
    {
        auto log = getLog();
        log->debug("Creating project directory {}",mPath);
        Directory d(mPath);
        if(!d.create())
        {
            log->error("Unable to create project directory {}",mPath);
            return false;
        }
        return true;
    }

    bool
    ProjectDirectory::createAllAssetDirectories
    ()
    {
        auto log = getLog();
        auto assetTypes = Constants::DREAM_ASSET_TYPES_MAP;
        for (auto typePair : assetTypes)
        {
            auto type = typePair.first;
            if (!createAssetTypeDirectory(type))
            {
                log->error("Unable to create asset directory {}",Constants::getAssetTypeStringFromTypeEnum(type));
                return false;
            }
        }
        return true;
    }

    vector<char>
    ProjectDirectory::readAssetData
    (AssetDefinition* assetDef, string format)
    {
        auto path = getAssetAbsolutePath(assetDef,format);
        File f(path);
        return f.readBinary();
    }

    bool
    ProjectDirectory::writeAssetData
    (AssetDefinition* assetDef, vector<char> data, string format)
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
    (string uuid)
    {
        auto assetDef = mProject->getDefinition()->getAssetDefinitionByUuid(uuid);
        if (assetDef)
        {
            stringstream path;
            path << mPath
                 << Constants::DIR_PATH_SEP
                 << assetDef->getType()
                 << Constants::DIR_PATH_SEP
                 << assetDef->getUuid()
                 << Constants::DIR_PATH_SEP
                 << assetDef->getFormat();
            return path.str();
        }
        return "";
    }


    string
    ProjectDirectory::getAssetAbsolutePath
    (AssetDefinition* assetDef, string format)
    {
        stringstream path;
        path << mPath
             << Constants::DIR_PATH_SEP
             << assetDef->getType()
             << Constants::DIR_PATH_SEP
             << assetDef->getUuid()
             << Constants::DIR_PATH_SEP
             << (format.empty() ? assetDef->getFormat() : format);
        return path.str();

    }

    string
    ProjectDirectory::getAssetAbsolutePath
    (AssetDefinition* assetDef)
    {
        return getAssetAbsolutePath(assetDef,"");
    }


    string
    ProjectDirectory::getAssetDirectoryPath
    (AssetDefinition* assetDef)
    {
        stringstream path;
        path << mPath
             << Constants::DIR_PATH_SEP
             << assetDef->getType()
             << Constants::DIR_PATH_SEP
             << assetDef->getUuid();
        return path.str();

    }

    bool
    ProjectDirectory::removeAssetDirectory
    (Dream::AssetDefinition* ad)
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
        auto pDef = mProject->getDefinition();
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
        ss << mPath
           << Constants::DIR_PATH_SEP
           << mProject->getDefinition()->getUuid()
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
    (AssetType type, string base)
    {
        stringstream ss;
        ss << (base.empty() ? mPath : base)
           << Constants::DIR_PATH_SEP
           << Constants::getAssetTypeStringFromTypeEnum(type);
        return ss.str();
    }

    bool ProjectDirectory::createAssetTypeDirectory(AssetType type)
    {
        string assetTypeDirPath = getAssetTypeDirectory(type);
        Directory dir(assetTypeDirPath);
        getLog()->debug("Creating asset dir {}", assetTypeDirPath);
        return dir.create();
    }

    vector<string>
    ProjectDirectory::cleanupAssetsDirectory
    ()
    {
        vector<string> retval;
        auto log = getLog();
        auto pDef = mProject->getDefinition();
        if (!pDef)
        {
            log->error("Cannot cleanup, no project definition");
            return retval;
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
                            retval.push_back(name);
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
        return retval;
    }

    Project*
    ProjectDirectory::newProject
    (string projectDir)
    {
        if (mProject)
        {
            closeProject();
        }

        if (directoryContainsProject(projectDir))
        {
            return nullptr;
        }

        mProject = new Project(this);
        mProject->setDefinition(ProjectDefinition::createNewProjectDefinition());
        mPath = projectDir;

        if(!baseDirectoryExists())
        {
            createBaseDirectory();
        }

        createAllAssetDirectories();

        saveProject();
        return mProject;
    }

    Project*
    ProjectDirectory::openFromFileReader
    (string projectPath, File &reader)
    {
        auto log = getLog();
        log->debug("Loading project from FileReader", reader.getPath());

        string projectJsonStr = reader.readString();

        if (projectJsonStr.empty())
        {
            log->error("Loading Failed. Project Content is Empty");
            return nullptr;
        }

        json projectJson;
        try
        {
            projectJson = json::parse(projectJsonStr);
        }
        catch (json::parse_error ex)
        {
            log->error("Exception while parsing project file: {}",ex.what());
            return nullptr;
        }

        mPath = projectPath;
        log->debug("Project path is: {}", mPath);
        mProject = new Project(this);
        auto pDef = new ProjectDefinition(projectJson);
        mProject->setDefinition(pDef);
        pDef->loadChildDefinitions();
        return mProject;
    }

    Project*
    ProjectDirectory::openFromDirectory
    (string directory)
    {
        auto log = getLog();

        string projectFileName = findProjectFileInDirectory(directory);

        if (projectFileName.size() == 0)
        {
            log->error( "Project: Error {} is not a valid project directory!", directory  );
            return nullptr;
        }

        log->debug( "Project: Loading {}{} from Directory {}", projectFileName , Constants::PROJECT_EXTENSION , directory );

        string projectFilePath = directory + Constants::PROJECT_PATH_SEP + projectFileName + Constants::PROJECT_EXTENSION;

        File projectFileReader(projectFilePath);

        return openFromFileReader(directory, projectFileReader);
    }

    void
    ProjectDirectory::closeProject
    ()
    {
        mPath = "";
        if (mProject)
        {
            delete mProject;
            mProject = nullptr;
        }
    }

    string
    ProjectDirectory::findProjectFileInDirectory
    (string directory)
    {
        auto log = getLog();
        Directory dir(directory);
        vector<string> directoryContents;
        directoryContents = dir.list();

        string projectFileName;

        for (string filename : directoryContents)
        {
            size_t dotJsonIndex = filename.find(Constants::PROJECT_EXTENSION);
            if (dotJsonIndex != string::npos)
            {
                projectFileName = filename.substr(0,dotJsonIndex);
                log->debug( "Found project file ",projectFileName );
                return projectFileName;
            }
        }
        return "";
    }

    bool
    ProjectDirectory::directoryContainsProject
    (string dir)
    {
        bool hasJsonFile = !findProjectFileInDirectory(dir).empty();
        bool hasAssetDirectories = findAssetDirectories(dir);
        return hasJsonFile || hasAssetDirectories;
    }

    bool
    ProjectDirectory::findAssetDirectories
    (string dir)
    {
        auto log = getLog();
        auto assetTypes = Constants::DREAM_ASSET_TYPES_MAP;
        for (auto typePair : assetTypes)
        {
            auto type = typePair.first;
            string assetDir = getAssetTypeDirectory(type,dir);
            log->error("Checking for {}",assetDir);
            if (!Directory(assetDir).exists())
            {
                return false;
            }
        }
        return true;

    }

    Project*
    ProjectDirectory::openFromArgumentParser
    (ArgumentParser &parser)
    {
        auto log = getLog();
        log->debug( "Project: Loading from ArgumentParser" );
        File projectFileReader(parser.getProjectFilePath());
        return openFromFileReader(parser.getProjectPath(), projectFileReader);
    }

    const size_t ProjectDirectory::BufferSize = 1024*1024;

}
