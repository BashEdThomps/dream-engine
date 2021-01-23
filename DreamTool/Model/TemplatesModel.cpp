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
    (DreamToolContext* context)
        :Model(context),
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
       StorageManager* fm = mContext->getStorageManager();
       Directory* dir = fm->openDirectory(path);
       auto retval = dir->list();
       fm->closeDirectory(dir);
       return retval;
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
        LOG_ERROR("TemplatesModel: Loading template from {}" , ss.str());
        StorageManager* fm = mContext->getStorageManager();
        File* templateFile = fm->openFile(ss.str());
        auto retval = templateFile->readString();
        fm->closeFile(templateFile);
        return retval;
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
