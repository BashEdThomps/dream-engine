#include "TemplatesModel.h"
#include <sstream>
#include "../../DreamCore/Common/File.h"
#include "../../DreamCore/Common/Directory.h"

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
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        stringstream ss;
        ss << getTemplatesDirectory(t)
           << Constants::DIR_PATH_SEP
           << templateName
           << Constants::DIR_PATH_SEP
           << format;
#ifdef DREAM_LOG
        log->error("Loading template from {}" , ss.str());
#endif
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
