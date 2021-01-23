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
        const static string PREFS_FILE_NAME;
        PreferencesModel(DreamToolContext* context);
        ~PreferencesModel();
        bool load(string projectPath);
        bool save();
    private:
        string getAbsolutePath();
        json mJson;
        string mProjectPath;
    };
}
