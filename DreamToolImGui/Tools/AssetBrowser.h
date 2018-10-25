#pragma once

#include "DTWidget.h"

using Dream::Project;

namespace DreamTool
{
    class PropertiesWindow;
    class AssetBrowser : public DTWidget
    {
    public:
        AssetBrowser(Project* parent, PropertiesWindow*);
        ~AssetBrowser() override;
        void draw() override;
    private:
        PropertiesWindow* mPropertiesWindowHandle;
    };
}
