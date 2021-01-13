#include "PreferencesModel.h"

using Dream::File;
using Dream::Constants;

namespace DreamTool
{
    PreferencesModel::PreferencesModel
    ()
    {
        LOG_TRACE("Constructing");
    }

    PreferencesModel::~PreferencesModel
    ()
    {
        LOG_TRACE("Destructing");
    }

    bool
    PreferencesModel::load
    (string projectPath)
    {
        mProjectPath = projectPath;
        File f(getAbsolutePath());
        if (f.exists())
        {
            mJson = json::parse(f.readString());
            return true;
        }
        return false;
    }

    bool
    PreferencesModel::save
    ()
    {
        File f(getAbsolutePath());
        return f.writeString(mJson.dump(1));
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
