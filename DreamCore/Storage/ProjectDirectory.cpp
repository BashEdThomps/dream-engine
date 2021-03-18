#include "ProjectDirectory.h"

#include "Project/ProjectDefinition.h"
#include "Project/Project.h"
#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Components/AssetDefinition.h"
#include "Directory.h"
#include "StorageManager.h"

#include <sstream>

using std::make_shared;
using std::stringstream;

namespace octronic::dream
{
    ProjectDirectory::ProjectDirectory
    (const weak_ptr<StorageManager>& fileManager)
        : mStorageManager(fileManager)
    {
        LOG_TRACE("ProjectDirectory: {}", __FUNCTION__);
    }

    ProjectDirectory::ProjectDirectory
    (const weak_ptr<StorageManager>& fileManager,
     const weak_ptr<Project>& proj)
        : mProject(proj),
          mStorageManager(fileManager)
    {
        LOG_TRACE("ProjectDirectory: {}", __FUNCTION__);
    }

    ProjectDirectory::~ProjectDirectory
    ()
    {
        LOG_TRACE("ProjectDirectory: {}", __FUNCTION__);
        closeProject();
    }

    bool
    ProjectDirectory::baseDirectoryExists
    ()
    const
    {
        auto smLock = mStorageManager.lock();
        shared_ptr<Directory> d = smLock->openDirectory(mPath).lock();
        bool retval = d->exists();
        smLock->closeDirectory(d);
        return retval;
    }

    bool
    ProjectDirectory::createBaseDirectory
    ()
    const
    {
        LOG_DEBUG("ProjectDirectory: Creating project directory {}",mPath);
        auto smLock = mStorageManager.lock();
        auto d = smLock->openDirectory(mPath).lock();
        if(!d->create())
        {
            LOG_ERROR("ProjectDirectory: Unable to create project directory {}",mPath);
            smLock->closeDirectory(d);
            return false;
        }
        smLock->closeDirectory(d);
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

    weak_ptr<File>
    ProjectDirectory::readAssetData
    (const weak_ptr<AssetDefinition>& assetDef, const string &format)
    const
    {
        auto path = getAssetAbsolutePath(assetDef,format);
        weak_ptr<File> f = mStorageManager.lock()->openFile(path);
        return f;
    }

    bool
    ProjectDirectory::writeAssetData
    (const weak_ptr<AssetDefinition>& assetDef,
     const vector<uint8_t>& data, const string& format)
    const
    {
        auto smLock = mStorageManager.lock();
        auto dataPath = getAssetDirectoryPath(assetDef);
        LOG_DEBUG("ProjectDirectory: Writing asset data to {}",dataPath);

        //Check target directory exists
        auto dir = smLock->openDirectory(dataPath).lock();
        if (!dir->exists())
        {
            LOG_DEBUG("ProjectDirectory: Asset path does not exist {}",dataPath);
            auto adLock = assetDef.lock();
            auto assetTypeEnum = Constants::getAssetTypeEnumFromString(adLock->getType());
            if (!assetTypeDirectoryExists(assetTypeEnum))
            {
                if(!createAssetTypeDirectory(assetTypeEnum))
                {
                    LOG_ERROR("ProjectDirectory: Unable to create asset type directory");
                    return false;
                }
            }
            if(!dir->create())
            {
                LOG_ERROR("ProjectDirectory: Unable to create asset path {}",dataPath);
                return false;
            }
        }
        auto path = getAssetAbsolutePath(assetDef,format);
        LOG_DEBUG("ProjectDirectory: Copying asset to {}",path);
        auto file = smLock->openFile(path).lock();
        bool retval = file->writeBinary(data);
        smLock->closeFile(file);
        return retval;
    }

    bool
    ProjectDirectory::writeAssetStringData
    (const weak_ptr<AssetDefinition>& assetDef,
     const string& data, const string& format)
    const
    {
        auto smLock = mStorageManager.lock();
        auto dataPath = getAssetDirectoryPath(assetDef);
        LOG_DEBUG("ProjectDirectory: Writing asset data to {}",dataPath);

        //Check target directory exists
        auto dir = smLock->openDirectory(dataPath).lock();
        if (!dir->exists())
        {
            LOG_DEBUG("ProjectDirectory: Asset path does not exist {}",dataPath);

            auto adLock = assetDef.lock();
            auto assetTypeEnum = Constants::getAssetTypeEnumFromString(adLock->getType());

            if (!assetTypeDirectoryExists(assetTypeEnum))
            {
                if(!createAssetTypeDirectory(assetTypeEnum))
                {
                    LOG_ERROR("ProjectDirectory: Unable to create asset type directory");
                    return false;
                }
            }

            if(!dir->create())
            {
                LOG_ERROR("ProjectDirectory: Unable to create asset path {}",dataPath);
                return false;
            }
        }
        auto path = getAssetAbsolutePath(assetDef,format);
        LOG_DEBUG("ProjectDirectory: Copying asset to {}",path);
        auto file = smLock->openFile(path).lock();
        bool retval = file->writeString(data);
        smLock->closeFile(file);
        return retval;
    }

    string
    ProjectDirectory::getAssetAbsolutePath
    (UuidType uuid)
    const
    {
        if (auto defLock = mProject->getDefinition().lock())
        {
            if (auto assetDef = defLock->getAssetDefinitionByUuid(uuid).lock())
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
        }
        return "";
    }


    string
    ProjectDirectory::getAssetAbsolutePath
    (const weak_ptr<AssetDefinition>& assetDef, const string& format)
    const
    {
        if(auto adLock = assetDef.lock())
        {
            stringstream path;
            path << mPath
                 << Constants::DIR_PATH_SEP
                 << adLock->getType()
                 << Constants::DIR_PATH_SEP
                 << adLock->getUuid()
                 << Constants::DIR_PATH_SEP
                 << (format.empty() ? adLock->getFormat() : format);
            return path.str();
        }
        return "";

    }

    string
    ProjectDirectory::getAssetAbsolutePath
    (const weak_ptr<AssetDefinition>& assetDef)
    const
    {
        return getAssetAbsolutePath(assetDef,"");
    }

    string
    ProjectDirectory::getAssetDirectoryPath
    (const weak_ptr<AssetDefinition>& assetDef)
    const
    {
        auto adLock = assetDef.lock();
        stringstream path;
        path << mPath
             << Constants::DIR_PATH_SEP
             << adLock->getType()
             << Constants::DIR_PATH_SEP
             << adLock->getUuid();
        return path.str();

    }

    bool
    ProjectDirectory::removeAssetDirectory
    (const weak_ptr<AssetDefinition>& ad)
    const
    {
        auto path = getAssetDirectoryPath(ad);
        auto smLock = mStorageManager.lock();
        LOG_DEBUG("ProjectDirectory: Removing asset directory {}",path);
        auto d =  smLock->openDirectory(path).lock();
        bool retval = d->deleteDirectory();
        smLock->closeDirectory(d);
        return retval;
    }

    bool
    ProjectDirectory::saveProject
    ()
    const
    {
        if (auto pDef = mProject->getDefinition().lock())
        {
            auto jsDef = pDef->toJson();
            auto jsonStr = jsDef.dump(1);
            auto path = getProjectFilePath();
            auto smLock = mStorageManager.lock();
            auto f = smLock->openFile(path).lock();
            bool retval = f->writeString(jsonStr);
            smLock->closeFile(f);
            return retval;
        }
        return false;
    }

    string
    ProjectDirectory::getProjectFilePath
    ()
    const
    {
        if (auto defLock = mProject->getDefinition().lock())
        {
            stringstream ss;
            ss << mPath
               << Constants::DIR_PATH_SEP
               << defLock->getUuid()
               << Constants::PROJECT_EXTENSION;
            return ss.str();
        }
        return "";
    }

    bool
    ProjectDirectory::assetTypeDirectoryExists
    (AssetType type)
    const
    {
        string dirPath = getAssetTypeDirectory(type);
        if (auto smLock = mStorageManager.lock())
        {
            if (auto dir = smLock->openDirectory(dirPath).lock())
            {
                bool retval = dir->exists();
                smLock->closeDirectory(dir);
                return retval;
            }
        }
        return false;
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
        auto smLock = mStorageManager.lock();
        auto dir = smLock->openDirectory(assetTypeDirPath).lock();
        LOG_DEBUG("ProjectDirectory: Creating asset dir {}", assetTypeDirPath);
        bool retval = dir->create();
        smLock->closeDirectory(dir);
        return retval;
    }

    vector<string>
    ProjectDirectory::cleanupAssetsDirectory
    ()
    const
    {
        vector<string> retval;
        if (auto pDef = mProject->getDefinition().lock())
        {
            LOG_ERROR("ProjectDirectory: Cannot cleanup, no project definition");
            return retval;

            for (const auto& typePair : Constants::DREAM_ASSET_TYPES_MAP)
            {
                AssetType type = typePair.first;
                string typeStr = Constants::getAssetTypeStringFromTypeEnum(type);
                string path = getAssetTypeDirectory(type);

                auto smLock = mStorageManager.lock();
                auto assetDir = smLock->openDirectory(path).lock();

                if (assetDir->exists())
                {
                    vector<string> subdirs = assetDir->listSubdirectories();
                    LOG_DEBUG("ProjectDirectory: Cleaning up {} containing {} definitions", path, subdirs.size());
                    int deletedCount=0;

                    for (string& subdirPath : subdirs)
                    {
                        auto subdir = smLock->openDirectory(subdirPath).lock();
                        if (subdir->exists())
                        {
                            UuidType name = static_cast<UuidType>(std::stoi(subdir->getName()));
                            LOG_DEBUG("ProjectDirectory: Checking subdir {} has definition",name);
                            if (auto def = pDef->getAssetDefinitionByUuid(name).lock())
                            {
                                LOG_DEBUG("ProjectDirectory: Definition {} does not exist, removing...",name);
                                subdir->deleteDirectory();
                                retval.push_back(subdirPath);
                            }
                        }
                        smLock->closeDirectory(subdir);
                    }
                    LOG_DEBUG("ProjectDirectory: Deleted {}/{} {} asset directories",deletedCount,subdirs.size(),typeStr);
                }
                else
                {
                    LOG_DEBUG("ProjectDirectory: No Directory {}",path);
                }
                smLock->closeDirectory(assetDir);
            }
        }
        return retval;
    }

    shared_ptr<Project>
    ProjectDirectory::newProject
    (const weak_ptr<Directory>& projectDir)
    {
        if (mProject)
        {
            closeProject();
        }

        if (directoryContainsProject(projectDir))
        {
            return nullptr;
        }

        if(auto pdLock = projectDir.lock())
        {
			mProject = make_shared<Project>(shared_from_this(), mStorageManager);
			mProject->setDefinition(ProjectDefinition::createNewProjectDefinition());
			mPath = pdLock->getPath();

			if(!baseDirectoryExists())
			{
				createBaseDirectory();
			}

			createAllAssetDirectories();

			saveProject();

        }
        return mProject;

    }

    shared_ptr<Project>
    ProjectDirectory::openFromFile
    (const weak_ptr<File>& file)
    {
        auto fLock = file.lock();
        LOG_DEBUG("ProjectDirectory: Loading project from FileReader", fLock->getPath());

        string projectJsonStr = fLock->readString();

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

        mPath = fLock->getDirectory();
        LOG_DEBUG("ProjectDirectory: Project path is: \"{}\"", mPath);
        mProject = make_shared<Project>(shared_from_this(), mStorageManager);
        auto pDef = make_shared<ProjectDefinition>(projectJson);
        mProject->setDefinition(pDef);
        pDef->loadChildDefinitions();
        return mProject;

    }

    shared_ptr<Project>
    ProjectDirectory::openFromDirectory
    (const weak_ptr<Directory>& directory)
    {
        string projectFileName = findProjectFileInDirectory(directory);
        auto dirLock = directory.lock();

        if (projectFileName.empty())
        {
            LOG_ERROR( "ProjectDirectory: Project: Error {} is not a valid project directory!", dirLock->getPath()  );
            return nullptr;
        }

        LOG_DEBUG( "ProjectDirectory: Project: Loading {}{} from Directory {}", projectFileName , Constants::PROJECT_EXTENSION , dirLock->getPath());

        auto smLock = mStorageManager.lock();
        string projectFilePath = dirLock->getPath() + Constants::PROJECT_PATH_SEP + projectFileName + Constants::PROJECT_EXTENSION;
        auto projectFile = smLock->openFile(projectFilePath).lock();
        shared_ptr<Project> retval = openFromFile(projectFile);
        smLock->closeFile(projectFile);
        return retval;
    }

    void
    ProjectDirectory::closeProject
    ()
    {
        mPath = "";
        if (mProject)
        {
            mProject.reset();
        }
    }

    string
    ProjectDirectory::findProjectFileInDirectory
    (const weak_ptr<Directory>& dir)
    const
    {
        auto dirLock = dir.lock();
        vector<string> directoryContents;
        directoryContents = dirLock->list();

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
    (const weak_ptr<Directory>& dir)
    const
    {
        bool hasJsonFile = !findProjectFileInDirectory(dir).empty();
        bool hasAssetDirectories = findAssetDirectories(dir);
        return hasJsonFile || hasAssetDirectories;
    }

    bool
    ProjectDirectory::findAssetDirectories
    (const weak_ptr<Directory>& dir)
    const
    {
        auto smLock = mStorageManager.lock();
        auto dirLock = dir.lock();
        auto assetTypes = Constants::DREAM_ASSET_TYPES_MAP;
        for (const auto &typePair : assetTypes)
        {
            auto type = typePair.first;
            string assetDirPath = getAssetTypeDirectory(type,dirLock->getName());
            auto assetDir = smLock->openDirectory(assetDirPath).lock();
            LOG_DEBUG("ProjectDirectory: Checking for {}",assetDir->getPath());
            if (!assetDir->exists())
            {
                smLock->closeDirectory(assetDir);
                return false;
            }
            smLock->closeDirectory(assetDir);
            assetDir = nullptr;
        }
        return true;
    }
}
