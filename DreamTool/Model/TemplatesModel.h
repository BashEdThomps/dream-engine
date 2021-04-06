#pragma once

#include "Model.h"

#include <DreamCore.h>

#include <string>
#include <vector>

using std::vector;
using std::string;
using octronic::dream::Directory;
using octronic::dream::AssetType;

namespace octronic::dream::tool
{
    class TemplatesModel : public Model
    {
    public:
        TemplatesModel(DreamToolContext& context);

        vector<string> getTemplateNames(AssetType t);
        string getTemplate(AssetType t, string templateName, string format);
    protected:
        string getTemplatesDirectory(AssetType t);
    private:
        const static string TEMPLATES_BASE_DIR;
        string mBaseDir;
    };
}
