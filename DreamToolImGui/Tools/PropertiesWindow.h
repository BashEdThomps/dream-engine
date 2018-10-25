#pragma once

#include "DTWidget.h"
#include "PropertyType.h"

using Dream::IDefinition;
using Dream::IRuntime;

namespace DreamTool
{
    class PropertiesWindow : public DTWidget
    {
    public:
        PropertiesWindow(Project*);
        ~PropertiesWindow() override;

        void draw() override;
        void setRuntime(IRuntime* runt);
        void setDefinition(IDefinition* def);
        void clear();
        void setPropertyType(PropertyType t);
        void drawProjectProperties();
        void drawSceneProperties();
        void drawSceneObjectProperties();
        void drawAssetProperties();
    private:
        void drawNameAndIdProperties();
    private:
        PropertyType mType;
        IDefinition* mDefinition;
        IRuntime* mRuntime;
    };

}
