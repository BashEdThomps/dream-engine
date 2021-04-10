#include "ProjectDirectory.h"

#include "Project/ProjectDefinition.h"
#include "Common/Logger.h"
#include "Components/AssetDefinition.h"
#include "Storage/Directory.h"
#include "Storage/StorageManager.h"

#include <sstream>

using std::make_shared;
using std::stringstream;

namespace octronic::dream
{
  ProjectDirectory::ProjectDirectory
  (StorageManager& storageManager, const string& baseDir)
    : mBasePath(baseDir),
      mStorageManager(storageManager)
  {
    LOG_TRACE("ProjectDirectory: {}", __FUNCTION__);
  }

  bool
  ProjectDirectory::baseDirectoryExists
  ()
  const
  {
    auto& sm = mStorageManager.get();
    auto& d = sm.openDirectory(mBasePath);
    bool retval = d.exists();
    sm.closeDirectory(d);
    return retval;
  }

  bool
  ProjectDirectory::createBaseDirectory
  ()
  const
  {
    LOG_DEBUG("ProjectDirectory: Creating project directory {}",mBasePath);
    auto& sm = mStorageManager.get();
    auto& d = sm.openDirectory(mBasePath);
    if(d.create())
    {
      sm.closeDirectory(d);
      return true;
    }
    LOG_ERROR("ProjectDirectory: Unable to create project directory {}",mBasePath);
    sm.closeDirectory(d);
    return false;
  }

  bool
  ProjectDirectory::createAllAssetDirectories
  ()
  const
  {
    auto assetTypes = AssetTypeHelper::TypesMap;
    for (const auto& typePair : assetTypes)
    {
      auto type = typePair.first;
      if (!createAssetTypeDirectory(type))
      {
        LOG_ERROR("ProjectDirectory: Unable to create asset directory {}",
                  AssetTypeHelper::GetAssetTypeStringFromTypeEnum(type));
        return false;
      }
    }
    return true;
  }

  File&
  ProjectDirectory::openAssetFile
  (AssetDefinition& assetDef,
   const string &format)
  const
  {
    auto path = getAssetAbsolutePath(assetDef,format);
    auto& sm = mStorageManager.get();
    return sm.openFile(path);
  }

  bool
  ProjectDirectory::writeAssetData
  (AssetDefinition& assetDef,
   const vector<uint8_t>& data,
   const string& format)
  const
  {
    auto assetTypeEnum = AssetTypeHelper::GetAssetTypeEnumFromString(assetDef.getType());
    auto dataPath = getAssetDirectoryPath(assetDef);

    LOG_DEBUG("ProjectDirectory: Writing asset data to {}",dataPath);

    //Check target directory exists
    auto& sm = mStorageManager.get();
    auto& dir = sm.openDirectory(dataPath);

    if (!dir.exists())
    {
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
    LOG_DEBUG("ProjectDirectory: Asset path does not exist {}",dataPath);
    auto path = getAssetAbsolutePath(assetDef,format);
    LOG_DEBUG("ProjectDirectory: Copying asset to {}",path);
    auto& file = sm.openFile(path);
    bool retval = file.writeBinary(data);
    sm.closeFile(file);
    return retval;
  }

  bool
  ProjectDirectory::writeAssetStringData
  (AssetDefinition& assetDef,
   const string& data,
   const string& format)
  const
  {
    auto dataPath = getAssetDirectoryPath(assetDef);
    LOG_DEBUG("ProjectDirectory: Writing asset data to {}",dataPath);

    //Check target directory exists
    auto& sm = mStorageManager.get();
    auto& dir = sm.openDirectory(dataPath);

    if (!dir.exists())
    {
      if(!dir.create())
      {
        LOG_ERROR("ProjectDirectory: Unable to create asset path {}",dataPath);
        sm.closeDirectory(dir);
        return false;
      }
    }
    auto path = getAssetAbsolutePath(assetDef,format);
    LOG_DEBUG("ProjectDirectory: Copying asset to {}",path);
    auto& file = sm.openFile(path);
    bool retval = file.writeString(data);
    sm.closeFile(file);
    return retval;
  }

  string
  ProjectDirectory::getAssetAbsolutePath
  (AssetDefinition& assetDef,
   const string& format)
  const
  {
    stringstream path;
    path << mBasePath
         << Constants::DIRECTORY_PATH_SEP
         << assetDef.getType()
         << Constants::DIRECTORY_PATH_SEP
         << assetDef.getUuid()
         << Constants::DIRECTORY_PATH_SEP
         << (format.empty() ? assetDef.getFormat() : format);
    return path.str();
  }

  string
  ProjectDirectory::getAssetAbsolutePath
  (AssetDefinition& assetDef)
  const
  {
    return getAssetAbsolutePath(assetDef,"");
  }

  string
  ProjectDirectory::getAssetDirectoryPath
  (AssetDefinition& assetDef)
  const
  {
    stringstream path;
    path << mBasePath
         << Constants::DIRECTORY_PATH_SEP
         << assetDef.getType()
         << Constants::DIRECTORY_PATH_SEP
         << assetDef.getUuid();
    return path.str();
  }

  bool
  ProjectDirectory::removeAssetDirectory
  (AssetDefinition& ad)
  const
  {
    auto& sm = mStorageManager.get();
    auto path = getAssetDirectoryPath(ad);
    LOG_DEBUG("ProjectDirectory: Removing asset directory {}",path);
    auto& d =  sm.openDirectory(path);
    bool retval = d.deleteDirectory();
    sm.closeDirectory(d);
    return retval;
  }

  bool
  ProjectDirectory::saveProject
  (ProjectDefinition& pDef)
  const
  {
    auto jsDef = pDef.getJson();
    auto jsonStr = jsDef.dump(1);
    auto path = getProjectFilePath(pDef);
    auto& sm = mStorageManager.get();
    auto& f = sm.openFile(path);
    bool retval = f.writeString(jsonStr);
    sm.closeFile(f);
    return retval;
  }

  string
  ProjectDirectory::getProjectFilePath
  (ProjectDefinition& pDef)
  const
  {
    stringstream ss;
    ss << mBasePath
       << Constants::DIRECTORY_PATH_SEP
       << pDef.getUuid()
       << Constants::PROJECT_FILE_EXTENSION;
    return ss.str();
  }

  bool
  ProjectDirectory::assetTypeDirectoryExists
  (AssetType type)
  const
  {
    string dirPath = getAssetTypeDirectory(type);
    auto& sm = mStorageManager.get();
    auto& dir = sm.openDirectory(dirPath);
    bool retval = dir.exists();
    sm.closeDirectory(dir);
    return retval;
  }

  string
  ProjectDirectory::getAssetTypeDirectory
  (AssetType type)
  const
  {
    stringstream ss;
    ss << mBasePath
       << Constants::DIRECTORY_PATH_SEP
       << AssetTypeHelper::GetAssetTypeStringFromTypeEnum(type);
    return ss.str();
  }

  bool
  ProjectDirectory::createAssetTypeDirectory
  (AssetType type)
  const
  {
    string assetTypeDirPath = getAssetTypeDirectory(type);
    auto& sm = mStorageManager.get();
    auto& dir = sm.openDirectory(assetTypeDirPath);
    LOG_DEBUG("ProjectDirectory: Creating asset dir {}", assetTypeDirPath);
    bool retval = dir.create();
    sm.closeDirectory(dir);
    return retval;
  }

  // TODO - Broken asset def lookup?
  vector<string>
  ProjectDirectory::removeUnusedAssetDirectories
  (ProjectDefinition& pDef)
  const
  {
    vector<string> retval;
    LOG_ERROR("ProjectDirectory: Cannot cleanup, no project definition");
    return retval;

    auto& sm = mStorageManager.get();

    for (const auto& typePair : AssetTypeHelper::TypesMap)
    {
      AssetType type = typePair.first;
      string typeStr = AssetTypeHelper::GetAssetTypeStringFromTypeEnum(type);
      string path = getAssetTypeDirectory(type);

      auto& assetDir = sm.openDirectory(path);
      if (assetDir.exists())
      {
        vector<string> subdirs = assetDir.listSubdirectories();
        LOG_DEBUG("ProjectDirectory: Cleaning up {} containing {} definitions", path, subdirs.size());
        int deletedCount=0;

        for (string& subdirPath : subdirs)
        {
          auto& subdir = sm.openDirectory(subdirPath);
          if (subdir.exists())
          {
            UuidType name = static_cast<UuidType>(std::stoi(subdir.getName()));
            LOG_DEBUG("ProjectDirectory: Checking subdir {} has definition",name);
            try
            {
              auto& def = pDef.getAssetDefinitionByUuid(type, name).value().get();
              LOG_DEBUG("ProjectDirectory: Found AssetDefinition {} in project", def.getNameAndUuidString());
            }
            catch (std::exception& ex)
            {
              LOG_DEBUG("ProjectDirectory: Definition {} does not exist, removing...",name);
              subdir.deleteDirectory();
              retval.push_back(subdirPath);
            }
          }
          sm.closeDirectory(subdir);
        }
        LOG_DEBUG("ProjectDirectory: Deleted {}/{} {} asset directories",deletedCount,subdirs.size(),typeStr);
        sm.closeDirectory(assetDir);
      }
    }
    return retval;
  }

  bool
  ProjectDirectory::directoryContainsProject
  ()
  const
  {
    bool hasJsonFile = !findProjectFileInDirectory(false).empty();
    bool hasAssetDirectories = checkAssetDirectoriesExist();
    return hasJsonFile && hasAssetDirectories;
  }

  bool
  ProjectDirectory::checkAssetDirectoriesExist
  ()
  const
  {
    auto& sm = mStorageManager.get();
    auto& assetTypes = AssetTypeHelper::TypesMap;
    for (const auto &typePair : assetTypes)
    {
      auto type = typePair.first;
      string assetDirPath = getAssetTypeDirectory(type);
      auto& assetDir = sm.openDirectory(assetDirPath);
      LOG_DEBUG("ProjectDirectory: Checking for {}",assetDir.getPath());
      if (assetDir.exists())
      {
        sm.closeDirectory(assetDir);
        return true;
      }
      sm.closeDirectory(assetDir);
    }
    return false;
  }

  json
  ProjectDirectory::readProjectDefinition
  ()
  const
  {
    auto& sm = mStorageManager.get();
    auto projectFile = findProjectFileInDirectory(false);
    if (!projectFile.empty())
    {
      stringstream ss;
      ss << mBasePath << Constants::DIRECTORY_PATH_SEP << projectFile;
      auto& file = sm.openFile(ss.str());
      if (!file.exists())
      {
        LOG_ERROR("ProjectDirectory: {} File does not exist",__FUNCTION__);
        throw std::runtime_error("Project file does not exist");
      }

      string projectJsonStr = file.readString();
      if (!projectJsonStr.empty())
      {
        try
        {
          json projectJson = json::parse(projectJsonStr);
          LOG_DEBUG("ProjectDirectory: Project path is: \"{}\"", mBasePath);
          return projectJson;
        }
        catch (const json::parse_error& ex)
        {
          LOG_ERROR("ProjectDirectory: Exception while parsing project file: {}",ex.what());
          throw std::runtime_error("Error parsing project json file");
        }
      }
      else
      {
        LOG_ERROR("ProjectDirectory: Loading Failed. Project Content is Empty");
      }
    }
    throw std::exception();
  }

  json
  ProjectDirectory::createProjectDefinition
  ()
  const
  {
    if (directoryContainsProject())
    {
      stringstream ss;
      ss << "Directory "<< mBasePath<< " already contains a project";
      throw std::runtime_error(ss.str());
    }

    json js;
    js[Constants::NAME] = Constants::PROJECT_DEFAULT_NAME;
    js[Constants::UUID] = Uuid::RandomUuid();
    js[Constants::PROJECT_STARTUP_SCENE] = Uuid::INVALID;
    js[Constants::PROJECT_ANIMATION_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_AUDIO_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_FONT_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_MATERIAL_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_MODEL_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_PATH_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_PHYSICS_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_SCRIPT_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_SHADER_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_TEXTURE_ASSET_ARRAY] = json::array();
    js[Constants::PROJECT_SCENE_ARRAY] = json::array();
    js[Constants::PROJECT_TEMPLATE_ENTITIES_ARRAY] = json::array();

    ProjectDefinition pTemp(js);

    if(!baseDirectoryExists())
    {
      createBaseDirectory();
    }

    createAllAssetDirectories();

    saveProject(pTemp);

    return js;

  }

  string
  ProjectDirectory::findProjectFileInDirectory
  (bool removeExtension)
  const
  {
    auto& sm = mStorageManager.get();
    auto& dir = sm.openDirectory(mBasePath);
    vector<string> directoryContents = dir.list();

    string projectFileName;

    for (const string &filename : directoryContents)
    {
      size_t dotJsonIndex = filename.find(Constants::PROJECT_FILE_EXTENSION);
      if (dotJsonIndex != string::npos)
      {
        if (removeExtension)
        {
          projectFileName = filename.substr(0,dotJsonIndex);
        }
        else
        {
          projectFileName = filename;
        }
        LOG_DEBUG( "ProjectDirectory: Found project file ",projectFileName );
        return projectFileName;
      }
    }
    return "";
  }

  string ProjectDirectory::getBasePath() const
  {
    return mBasePath;
  }
}
