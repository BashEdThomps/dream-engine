#include "ProjectDirectory.h"

#include "Project/Project.h"
#include "ProjectDefinition.h"
#include "Common/Directory.h"
#include "Common/File.h"
#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Components/AssetDefinition.h"

#include <sstream>

using std::stringstream;

namespace Dream
{
    ProjectDirectory::ProjectDirectory()
        : mProject(nullptr)
    {

    }

    ProjectDirectory::ProjectDirectory
    (Project* proj)
        : mProject(proj)
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
    const
    {
        Directory d(mPath);
        return d.exists();
    }

    bool
    ProjectDirectory::createBaseDirectory
    ()
    const
    {
        LOG_DEBUG("ProjectDirectory: Creating project directory {}",mPath);
        Directory d(mPath);
        if(!d.create())
        {
            LOG_ERROR("ProjectDirectory: Unable to create project directory {}",mPath);
            return false;
        }
        return true;
    }

    bool
    ProjectDirectory::createAllAssetDirectories
    ()
    const
    {
        auto assetTypes = Constants::DREAM_ASSET_TYPES_MAP;
        for (const auto& typePair : assetTypes)
        {
            auto type = typePair.first;
            if (!createAssetTypeDirectory(type))
            {
                LOG_ERROR("ProjectDirectory: Unable to create asset directory {}",Constants::getAssetTypeStringFromTypeEnum(type));
                return false;
            }
        }
        return true;
    }

    vector<char>
    ProjectDirectory::readAssetData
    (AssetDefinition* assetDef, const string &format)
    const
    {
        auto path = getAssetAbsolutePath(assetDef,format);
        File f(path);
        return f.readBinary();
    }

    bool
    ProjectDirectory::writeAssetData
    (AssetDefinition* assetDef, const vector<char>& data, const string& format)
    const
    {
        auto dataPath = getAssetDirectoryPath(assetDef);
        LOG_DEBUG("ProjectDirectory: Writing asset data to {}",dataPath);

        //Check target directory exists
        Directory dir(dataPath);
        if (!dir.exists())
        {
            LOG_DEBUG("ProjectDirectory: Asset path does not exist {}",dataPath);
            auto assetTypeEnum = Constants::getAssetTypeEnumFromString(assetDef->getType());
            if (!assetTypeDirectoryExists(assetTypeEnum))
            {
                if(!createAssetTypeDirectory(assetTypeEnum))
                {
                    LOG_ERROR("ProjectDirectory: Unable to create asset type directory");
                    return false;
                }
            }
            if(!dir.create())
            {
                LOG_ERROR("ProjectDirectory: Unable to create asset path {}",dataPath);
                return false;
            }
        }
        auto path = getAssetAbsolutePath(assetDef,format);
        LOG_DEBUG("ProjectDirectory: Copying asset to {}",path);
        auto file = File(path);
        return file.writeBinary(data);
    }

    string
    ProjectDirectory::getAssetAbsolutePath
    (uint32_t uuid)
    const
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
    (AssetDefinition* assetDef, const string& format)
    const
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
    const
    {
        return getAssetAbsolutePath(assetDef,"");
    }


    string
    ProjectDirectory::getAssetDirectoryPath
    (AssetDefinition* assetDef)
    const
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
    (AssetDefinition* ad)
    const
    {
        auto path = getAssetDirectoryPath(ad);
        LOG_DEBUG("ProjectDirectory: Removing asset directory {}",path);
        Directory d(path);
        return d.deleteDirectory();
    }

    bool
    ProjectDirectory::saveProject
    ()
    const
    {
        auto pDef = mProject->getDefinition();
        if (pDef)
        {
            auto jsDef = pDef->getJson();
            auto jsonStr = jsDef.dump();
            auto path = getProjectFilePath();
            File f(path);
            return f.writeString(jsonStr);
        }
        return false;
    }

    string
    ProjectDirectory::getProjectFilePath
    ()
    const
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
    const
    {
        string dirPath = getAssetTypeDirectory(type);
        Directory dir(dirPath);
        return dir.exists();
    }

    string
    ProjectDirectory::getAssetTypeDirectory
    (AssetType type, const string& base)
    const
    {
        stringstream ss;
        ss << (base.empty() ? mPath : base)
           << Constants::DIR_PATH_SEP
           << Constants::getAssetTypeStringFromTypeEnum(type);
        return ss.str();
    }

    bool
    ProjectDirectory::createAssetTypeDirectory
    (AssetType type)
    const
    {
        string assetTypeDirPath = getAssetTypeDirectory(type);
        Directory dir(assetTypeDirPath);
        LOG_DEBUG("ProjectDirectory: Creating asset dir {}", assetTypeDirPath);
        return dir.create();
    }

    vector<uint32_t>
    ProjectDirectory::cleanupAssetsDirectory
    ()
    const
    {
        vector<uint32_t> retval;
        auto pDef = mProject->getDefinition();
        if (!pDef)
        {
            LOG_ERROR("ProjectDirectory: Cannot cleanup, no project definition");
            return retval;
        }

        for (const auto& typePair : Constants::DREAM_ASSET_TYPES_MAP)
        {
            AssetType type = typePair.first;
            string typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
            string path = getAssetTypeDirectory(type);
            Directory assetDir(path);
            if (assetDir.exists())
            {
                auto subdirs = assetDir.listSubdirectories();
                LOG_ERROR("ProjectDirectory: Cleaning up {} containing {} definitions", path, subdirs.size());
                int deletedCount=0;
                for (const auto& subdirPath : subdirs)
                {
                    Directory subdir(subdirPath);
                    if (subdir.exists())
                    {
                        uint32_t name = static_cast<uint32_t>(std::stoi(subdir.getName()));
                        LOG_ERROR("ProjectDirectory: Checking subdir {} has definition",name);
                        auto def = pDef->getAssetDefinitionByUuid(name);
                        if (!def)
                        {
                            LOG_ERROR("ProjectDirectory: Definition {} does not exist, removing...",name);
                            subdir.deleteDirectory();
                            retval.push_back(name);
                        }
                    }
                }
                LOG_ERROR("ProjectDirectory: Deleted {}/{} {} asset directories",deletedCount,subdirs.size(),typeStr);
            }
            else
            {
                LOG_ERROR("ProjectDirectory: No Directory {}",path);
            }
        }
        return retval;
    }

    Project*
    ProjectDirectory::newProject
    (const string &projectDir)
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
    (const string &projectPath, const File &reader)
    {
        LOG_DEBUG("ProjectDirectory: Loading project from FileReader", reader.getPath());

        string projectJsonStr = reader.readString();

        if (projectJsonStr.empty())
        {
            LOG_ERROR("ProjectDirectory: Loading Failed. Project Content is Empty");
            return nullptr;
        }

        json projectJson;
        try
        {
            projectJson = json::parse(projectJsonStr);
        }
        catch (const json::parse_error& ex)
        {
            LOG_ERROR("ProjectDirectory: Exception while parsing project file: {}",ex.what());
            return nullptr;
        }

        mPath = projectPath;
        LOG_DEBUG("ProjectDirectory: Project path is: {}", mPath);
        mProject = new Project(this);
        auto pDef = new ProjectDefinition(projectJson);
        mProject->setDefinition(pDef);
        pDef->loadChildDefinitions();
        return mProject;
    }

    Project*
    ProjectDirectory::openFromDirectory
    (const string &directory)
    {
        string projectFileName = findProjectFileInDirectory(directory);

        if (projectFileName.empty())
        {
            LOG_ERROR( "ProjectDirectory: Project: Error {} is not a valid project directory!", directory  );
            return nullptr;
        }

        LOG_DEBUG( "ProjectDirectory: Project: Loading {}{} from Directory {}", projectFileName , Constants::PROJECT_EXTENSION , directory );

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
    (const string& directory)
    const
    {
        Directory dir(directory);
        vector<string> directoryContents;
        directoryContents = dir.list();

        string projectFileName;

        for (const string &filename : directoryContents)
        {
            size_t dotJsonIndex = filename.find(Constants::PROJECT_EXTENSION);
            if (dotJsonIndex != string::npos)
            {
                projectFileName = filename.substr(0,dotJsonIndex);
                LOG_DEBUG( "ProjectDirectory: Found project file ",projectFileName );
                return projectFileName;
            }
        }
        return "";
    }

    bool
    ProjectDirectory::directoryContainsProject
    (const string& dir)
    const
    {
        bool hasJsonFile = !findProjectFileInDirectory(dir).empty();
        bool hasAssetDirectories = findAssetDirectories(dir);
        return hasJsonFile || hasAssetDirectories;
    }

    bool
    ProjectDirectory::findAssetDirectories
    (const string &dir)
    const
    {
        auto assetTypes = Constants::DREAM_ASSET_TYPES_MAP;
        for (const auto &typePair : assetTypes)
        {
            auto type = typePair.first;
            string assetDir = getAssetTypeDirectory(type,dir);
            LOG_ERROR("ProjectDirectory: Checking for {}",assetDir);
            if (!Directory(assetDir).exists())
            {
                return false;
            }
        }
        return true;
    }

    Project*
    ProjectDirectory::openFromArgumentParser
    (const ArgumentParser &parser)
    {
        LOG_DEBUG( "ProjectDirectory: Project: Loading from ArgumentParser" );
        File projectFileReader(parser.getProjectFilePath());
        return openFromFileReader(parser.getProjectPath(), projectFileReader);
    }

    const size_t ProjectDirectory::BufferSize = 1024*1024;
}
