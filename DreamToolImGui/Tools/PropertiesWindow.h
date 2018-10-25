#pragma once

#include "DTWidget.h"

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
    };

}
