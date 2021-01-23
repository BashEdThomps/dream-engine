#include "PreferencesModel.h"

#include <DreamCore.h>
#include "DreamToolContext.h"

using octronic::dream::File;
using octronic::dream::StorageManager;
using octronic::dream::Constants;

namespace octronic::dream::tool
{
    PreferencesModel::PreferencesModel
    (DreamToolContext* context)
        : Model(context)
    {
        LOG_TRACE("PreferencesModel: Constructing");
    }

    PreferencesModel::~PreferencesModel
    ()
    {
        LOG_TRACE("PreferencesModel: Destructing");
    }

    bool
    PreferencesModel::load
    (string projectPath)
    {
        mProjectPath = projectPath;
        StorageManager* fm = mContext->getStorageManager();
        File* f = fm->openFile (getAbsolutePath());
        if (f->exists())
        {
            mJson = json::parse(f->readString());
            fm->closeFile(f);
            return true;
        }
        fm->closeFile(f);
        return false;
    }

    bool
    PreferencesModel::save
    ()
    {
        StorageManager* fm = mContext->getStorageManager();
        File* f = fm->openFile(getAbsolutePath());
        bool retval = f->writeString(mJson.dump(1));
        fm->closeFile(f);
        return retval;
    }

    string
    PreferencesModel::getAbsolutePath
    ()
    {
        stringstream str;
        str << mProjectPath;
        str << Constants::DIR_PATH_SEP;
        str << PREFS_FILE_NAME;
        return str.str();
    }

    const string PreferencesModel::PREFS_FILE_NAME = "DTPrefs.json";
}
