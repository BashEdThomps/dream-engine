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
        #ifdef DREAM_LOG
        getLog()->debug("Creating project directory {}",mPath);
        #endif
        Directory d(mPath);
        if(!d.create())
        {
            #ifdef DREAM_LOG
            getLog()->error("Unable to create project directory {}",mPath);
            #endif
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
        for (auto typePair : assetTypes)
        {
            auto type = typePair.first;
            if (!createAssetTypeDirectory(type))
            {
                #ifdef DREAM_LOG
                getLog()->error("Unable to create asset directory {}",Constants::getAssetTypeStringFromTypeEnum(type));
                #endif
                return false;
            }
        }
        return true;
    }

    vector<char>
    ProjectDirectory::readAssetData
    (AssetDefinition* assetDef, string format)
    const
    {
        auto path = getAssetAbsolutePath(assetDef,format);
        File f(path);
        return f.readBinary();
    }

    bool
    ProjectDirectory::writeAssetData
    (AssetDefinition* assetDef, vector<char> data, string format)
    const
    {
        auto dataPath = getAssetDirectoryPath(assetDef);
        #ifdef DREAM_LOG
        getLog()->debug("Writing asset data to {}",dataPath);
        #endif

        //Check target directory exists
        Directory dir(dataPath);
        if (!dir.exists())
        {
            #ifdef DREAM_LOG
            getLog()->debug("Asset path does not exist {}",dataPath);
            #endif
            auto assetTypeEnum = Constants::getAssetTypeEnumFromString(assetDef->getType());
            if (!assetTypeDirectoryExists(assetTypeEnum))
            {
                if(!createAssetTypeDirectory(assetTypeEnum))
                {
                    #ifdef DREAM_LOG
                    getLog()->error("Unable to create asset type directory");
                    #endif
                    return false;
                }
            }
            if(!dir.create())
            {
                #ifdef DREAM_LOG
                getLog()->error("Unable to create asset path {}",dataPath);
                #endif
                return false;
            }
        }
        auto path = getAssetAbsolutePath(assetDef,format);
        #ifdef DREAM_LOG
        getLog()->debug("Copying asset to {}",path);
        #endif
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
    (AssetDefinition* assetDef, string format)
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
        #ifdef DREAM_LOG
        getLog()->debug("Removing asset directory {}",path);
        #endif
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
    (AssetType type, string base)
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
        #ifdef DREAM_LOG
        getLog()->debug("Creating asset dir {}", assetTypeDirPath);
        #endif
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
            #ifdef DREAM_LOG
            getLog()->error("Cannot cleanup, no project definition");
            #endif
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
                #ifdef DREAM_LOG
                getLog()->error("Cleaning up {} containing {} definitions", path, subdirs.size());
                #endif
                int deletedCount=0;
                for (auto subdirPath : subdirs)
                {
                    Directory subdir(subdirPath);
                    if (subdir.exists())
                    {
                        uint32_t name = static_cast<uint32_t>(std::stoi(subdir.getName()));
                        #ifdef DREAM_LOG
                        getLog()->error("Checking subdir {} has definition",name);
                        #endif
                        auto def = pDef->getAssetDefinitionByUuid(name);
                        if (!def)
                        {
                            #ifdef DREAM_LOG
                            getLog()->error("Definition {} does not exist, removing...",name);
                            #endif
                            subdir.deleteDirectory();
                            retval.push_back(name);
                        }
                    }
                }
                #ifdef DREAM_LOG
                getLog()->error("Deleted {}/{} {} asset directories",deletedCount,subdirs.size(),typeStr);
                #endif
            }
            else
            {
                #ifdef DREAM_LOG
                getLog()->error("No Directory {}",path);
                #endif
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
        #ifdef DREAM_LOG
        getLog()->debug("Loading project from FileReader", reader.getPath());
        #endif

        string projectJsonStr = reader.readString();

        if (projectJsonStr.empty())
        {
            #ifdef DREAM_LOG
            getLog()->error("Loading Failed. Project Content is Empty");
            #endif
            return nullptr;
        }

        json projectJson;
        try
        {
            projectJson = json::parse(projectJsonStr);
        }
        catch (json::parse_error ex)
        {
            #ifdef DREAM_LOG
            getLog()->error("Exception while parsing project file: {}",ex.what());
            #endif
            return nullptr;
        }

        mPath = projectPath;
        #ifdef DREAM_LOG
        getLog()->debug("Project path is: {}", mPath);
        #endif
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

        if (projectFileName.size() == 0)
        {
            #ifdef DREAM_LOG
            getLog()->error( "Project: Error {} is not a valid project directory!", directory  );
            #endif
            return nullptr;
        }

        #ifdef DREAM_LOG
        getLog()->debug( "Project: Loading {}{} from Directory {}", projectFileName , Constants::PROJECT_EXTENSION , directory );
        #endif

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

        for (string filename : directoryContents)
        {
            size_t dotJsonIndex = filename.find(Constants::PROJECT_EXTENSION);
            if (dotJsonIndex != string::npos)
            {
                projectFileName = filename.substr(0,dotJsonIndex);
                #ifdef DREAM_LOG
                getLog()->debug( "Found project file ",projectFileName );
                #endif
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
        for (auto typePair : assetTypes)
        {
            auto type = typePair.first;
            string assetDir = getAssetTypeDirectory(type,dir);
            #ifdef DREAM_LOG
            getLog()->error("Checking for {}",assetDir);
            #endif
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
        #ifdef DREAM_LOG
        getLog()->debug( "Project: Loading from ArgumentParser" );
        #endif
        File projectFileReader(parser.getProjectFilePath());
        return openFromFileReader(parser.getProjectPath(), projectFileReader);
    }

    const size_t ProjectDirectory::BufferSize = 1024*1024;
}
