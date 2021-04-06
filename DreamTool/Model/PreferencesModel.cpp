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
  (DreamToolContext& context)
    : Model(context)
  {
    LOG_TRACE("PreferencesModel: {}", __FUNCTION__);
  }

  bool
  PreferencesModel::load
  ()
  {
    auto& sm = getContext().getStorageManager();
    auto& f = sm.openFile(PREFS_FILE_NAME);
    if (f.exists())
    {
      mJson = json::parse(f.readString());
      sm.closeFile(f);
      return true;
    }
    sm.closeFile(f);
    return false;
  }

  bool
  PreferencesModel::save
  ()
  {
    auto& sm = getContext().getStorageManager();
    auto& f = sm.openFile(PREFS_FILE_NAME);
    bool retval = f.writeString(mJson.dump(1));
    sm.closeFile(f);
    return retval;
  }

  string
  PreferencesModel::getExternalEditorPath
  ()
  const
  {
    if (mJson.find(PREFS_EXTERNAL_EDITOR_PATH) == mJson.end())
    {
      return "";
    }
    return mJson[PREFS_EXTERNAL_EDITOR_PATH];
  }

  bool
  PreferencesModel::hasExternalEditorPath
  ()
  {
    return !getExternalEditorPath().empty();
  }

  void
  PreferencesModel::setExternalEditorPath
  (const string& editor)
  {
    mJson[PREFS_EXTERNAL_EDITOR_PATH] = editor;
  }
}
