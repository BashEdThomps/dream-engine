#pragma once

#include "Model.h"

#include <DreamCore.h>
#include <json.hpp>
#include <string>

using nlohmann::json;
using std::string;

namespace octronic::dream::tool
{
    class PreferencesModel : public Model
    {
    public:

        PreferencesModel(DreamToolContext* context);
        ~PreferencesModel();

        string getExternalEditorPath();
        bool hasExternalEditorPath();
        void setExternalEditorPath(const string& path);

        bool load();
        bool save();

    private: // Variables
        json mJson;
    private: // statics
        const static string PREFS_FILE_NAME;
        const static string PREFS_EXTERNAL_EDITOR_PATH;
    };
}
