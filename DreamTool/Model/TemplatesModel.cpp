#include "TemplatesModel.h"
#include <sstream>

namespace DreamTool
{
    TemplatesModel::TemplatesModel
    ()
        :DreamObject ("TemplatesDirectoryModel"),
          mBaseDir("./templates")
    {

    }

    TemplatesModel::~TemplatesModel()
    {

    }

    vector<string>
    TemplatesModel::getTemplateNames
    (AssetType t)
    {
       auto path = getTemplatesDirectory(t);
       Directory dir(path);
       return dir.list();
    }

    string
    TemplatesModel::getTemplate
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
    TemplatesModel::getTemplatesDirectory
    (AssetType t)
    {
        stringstream ss;
        ss << mBaseDir
           << Constants::DIR_PATH_SEP
           << Constants::getAssetTypeStringFromTypeEnum(t);
        return ss.str();
    }
}
