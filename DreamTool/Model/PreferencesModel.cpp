#include "PreferencesModel.h"

#include <DreamCore.h>
#include "DreamToolContext.h"

using octronic::dream::File;
using octronic::dream::StorageManager;
using octronic::dream::Constants;

namespace octronic::dream::tool
{
    const string PreferencesModel::PREFS_FILE_NAME = "DTPrefs.json";
    const string PreferencesModel::PREFS_EXTERNAL_EDITOR_PATH = "external_editor_path";

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
    ()
    {
        StorageManager* fm = mContext->getStorageManager();
        File* f = fm->openFile (PREFS_FILE_NAME);
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
        File* f = fm->openFile(PREFS_FILE_NAME);
        bool retval = f->writeString(mJson.dump(1));
        fm->closeFile(f);
        return retval;
    }

    string PreferencesModel::getExternalEditorPath()
    {
       if (mJson.find(PREFS_EXTERNAL_EDITOR_PATH) == mJson.end())
       {
           mJson[PREFS_EXTERNAL_EDITOR_PATH] = "";
       }
       return mJson[PREFS_EXTERNAL_EDITOR_PATH];
    }

    void PreferencesModel::setExternalEditorPath(const string& editor)
    {
           mJson[PREFS_EXTERNAL_EDITOR_PATH] = editor;
    }
}
