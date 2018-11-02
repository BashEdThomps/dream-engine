#include "TemplatesDirectoryModel.h"
#include <sstream>

namespace DreamTool{
    TemplatesDirectoryModel::TemplatesDirectoryModel
    ()
        :DreamObject ("TemplatesDirectoryModel"),
          mBaseDir("./templates")
    {

    }

    TemplatesDirectoryModel::~TemplatesDirectoryModel()
    {

    }

    vector<string>
    TemplatesDirectoryModel::getTemplateNames
    (AssetType t)
    {
       auto path = getTemplatesDirectory(t);
       Directory dir(path);
       return dir.list();
    }

    string
    TemplatesDirectoryModel::getTemplate
    (AssetType t, string templateName, string format)
    {
        auto log = getLog();
        stringstream ss;
        ss << getTemplatesDirectory(t)
           << Constants::DIR_PATH_SEP
           << templateName
           << Constants::DIR_PATH_SEP
           << format;
        log->error("Loading template from {}" , ss.str());
        File templateFile(ss.str());
        return templateFile.readString();
    }

    string
    TemplatesDirectoryModel::getTemplatesDirectory
    (AssetType t)
    {
        stringstream ss;
        ss << mBaseDir
           << Constants::DIR_PATH_SEP
           << Constants::getAssetTypeStringFromTypeEnum(t);
        return ss.str();
    }
}
