#include "TemplatesModel.h"

#include <sstream>

using std::stringstream;
using Dream::File;
using Dream::Constants;

namespace DreamTool
{
    TemplatesModel::TemplatesModel()
        :mBaseDir("./templates")
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
        stringstream ss;
        ss << getTemplatesDirectory(t)
           << Constants::DIR_PATH_SEP
           << templateName
           << Constants::DIR_PATH_SEP
           << format;
        LOG_ERROR("Loading template from {}" , ss.str());
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
