#include "TemplatesModel.h"

#include "DreamToolContext.h"

#include <sstream>

using std::stringstream;
using octronic::dream::File;
using octronic::dream::StorageManager;
using octronic::dream::Constants;

namespace octronic::dream::tool
{
  TemplatesModel::TemplatesModel
  (DreamToolContext& context)
    :Model(context),
      mBaseDir(TEMPLATES_BASE_DIR)
  {
    LOG_TRACE("TemplatesModel: {}",__FUNCTION__);
  }

  vector<string>
  TemplatesModel::getTemplateNames
  (AssetType t)
  {
    static string no_dot_files_regex = "^[^.][\\s\\S]+";

    auto path = getTemplatesDirectory(t);
    auto& sm = getContext().getStorageManager();
    auto& dir = sm.openDirectory(path);
    auto retval = dir.list(no_dot_files_regex);
    sm.closeDirectory(dir);
    return retval;
  }

  string
  TemplatesModel::getTemplate
  (AssetType t, string templateName, string format)
  {
    stringstream ss;
    ss << getTemplatesDirectory(t)
       << Constants::DIRECTORY_PATH_SEP
       << templateName
       << Constants::DIRECTORY_PATH_SEP
       << format;
    LOG_DEBUG("TemplatesModel: Loading template from {}" , ss.str());
    auto& sm = getContext().getStorageManager();
    auto& templateFile = sm.openFile(ss.str());
    if (templateFile.exists())
    {
      auto retval = templateFile.readString();
      sm.closeFile(templateFile);
      return retval;
    }
    LOG_DEBUG("TemplatesModel: Template file {} does not exist" , ss.str());
    return "";
  }

  string
  TemplatesModel::getTemplatesDirectory
  (AssetType t)
  {
    stringstream ss;
    ss << mBaseDir
       << Constants::DIRECTORY_PATH_SEP
       << AssetTypeHelper::GetAssetTypeStringFromTypeEnum(t);
    return ss.str();
  }


  const string TemplatesModel::TEMPLATES_BASE_DIR  = "./templates";
}
