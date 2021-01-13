#pragma once

#include <DreamCore.h>
#include <json.hpp>
#include <string>

using nlohmann::json;
using std::string;

namespace DreamTool
{
    class PreferencesModel
    {
    public:
        const static string PREFS_FILE_NAME;
        PreferencesModel();
        ~PreferencesModel();
        bool load(string projectPath);
        bool save();
    private:
        string getAbsolutePath();
        json mJson;
        string mProjectPath;
    };
}
