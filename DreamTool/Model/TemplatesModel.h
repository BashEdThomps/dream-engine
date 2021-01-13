#pragma once

#include <DreamCore.h>

#include <string>
#include <vector>

using std::vector;
using std::string;
using Dream::Directory;
using Dream::AssetType;

namespace DreamTool
{
    class TemplatesModel
    {
    public:
        TemplatesModel();
        ~TemplatesModel();

        vector<string> getTemplateNames(AssetType t);
        string getTemplate(AssetType t, string templateName, string format);
    protected:
        string getTemplatesDirectory(AssetType t);
    private:
        string mBaseDir;
    };
}
