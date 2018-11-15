#pragma once

#include "../../DreamCore/Common/DreamObject.h"
#include "../../DreamCore/deps/json/json.hpp"
#include <sstream>

using namespace Dream;
using namespace nlohmann;

namespace DreamTool
{
    class PreferencesModel : public DreamObject
    {
    public:
        const static string PREFS_FILE_NAME;
        PreferencesModel();
        ~PreferencesModel() override;
        bool load(string projectPath);
        bool save();
    private:
        string getAbsolutePath();
        json mJson;
        string mProjectPath;
    };
}
