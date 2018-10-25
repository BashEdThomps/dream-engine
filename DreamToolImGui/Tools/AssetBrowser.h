#pragma once

#include "DTWidget.h"

using Dream::Project;

namespace DreamTool
{

    class AssetBrowser : public DTWidget
    {
    public:
        AssetBrowser(Project* parent);
        ~AssetBrowser() override;
        void draw() override;
    };
}
