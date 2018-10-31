#pragma once

#include <DreamCore.h>

using namespace Dream;

namespace DreamTool
{
    class TemplatesDirectoryModel : public DreamObject
    {
    public:
        TemplatesDirectoryModel();
        ~TemplatesDirectoryModel();

        vector<string> getTemplateNames(AssetType t);
        string getTemplate(AssetType t, string templateName, string format);
    protected:
        string getTemplatesDirectory(AssetType t);
    private:
        string mBaseDir;
    };
}
