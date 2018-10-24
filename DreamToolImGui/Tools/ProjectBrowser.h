#include "DTWidget.h"

using Dream::DreamObject;

namespace DreamTool
{
    class ProjectBrowser : public DTWidget
    {
    public:
        ProjectBrowser(Project* proj);
        ~ProjectBrowser() override;
        void draw() override;
    };
}
