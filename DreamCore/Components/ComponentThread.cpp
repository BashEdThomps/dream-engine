#include "ComponentThread.h"
#include "IComponent.h"
#include <spdlog/spdlog.h>

using std::dynamic_pointer_cast;

namespace Dream
{
    ComponentThread::ComponentThread(shared_ptr<IComponent> component)
        : thread(&IComponent::updateComponent,component),
          DreamObject("ComponentThread"),
          mComponent(component)

    {
        auto log = getLog();
        log->trace(
            "Constructing Update Thread for {}",
            dynamic_pointer_cast<DreamObject>(component)->getClassName()
        );
    }

    ComponentThread::~ComponentThread()
    {
       auto log=getLog();
       log->trace("Destructing");
    }
}
