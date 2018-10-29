#include "ProjectDirectoryModel.h"
#include <sstream>

using Dream::File;
using Dream::Directory;
using Dream::Constants;
using std::stringstream;

namespace DreamTool
{
    ProjectDirectoryModel::ProjectDirectoryModel
    (Project* proj)
        : DreamObject("ProjectDirectoryModel"),
          mProject(proj)
    {

    }

    ProjectDirectoryModel::~ProjectDirectoryModel
    ()
    {
    }

    bool
    ProjectDirectoryModel::baseDirectoryExists
    ()
    {
        Directory d(mProject->getProjectPath());
        return d.exists();
    }

    bool
    ProjectDirectoryModel::createBaseDirectory
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
    ProjectDirectoryModel::createAllAssetDirectories
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
    ProjectDirectoryModel::createAssetDirectory
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
    ProjectDirectoryModel::readAssetData
    (IAssetDefinition* assetDef, string format)
    {
        auto path = getAssetAbsolutePath(assetDef,format);
        File f(path);
        return f.readBinary();
    }

    bool
    ProjectDirectoryModel::writeAssetData
    (IAssetDefinition* assetDef, vector<char> data, string format)
    {
        auto path = getAssetAbsolutePath(assetDef,format);
        auto file = File(path);
        return file.writeBinary(data);
    }

    string
    ProjectDirectoryModel::getAssetAbsolutePath
    (IAssetDefinition* assetDef, string format)
    {
        stringstream path;
        path << mProject->getProjectPath()
             << Constants::DIR_PATH_SEP
             << assetDef->getType()
             << Constants::DIR_PATH_SEP
             << (format.empty() ? assetDef->getFormat() : format);
        return path.str();

    }

    const size_t ProjectDirectoryModel::BufferSize = 1024*1024;
}
