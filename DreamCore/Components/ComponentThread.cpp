#include "ComponentThread.h"
#include "IComponent.h"
#include <spdlog/spdlog.h>

namespace Dream
{
    ComponentThread::ComponentThread(shared_ptr<IComponent> component)
        : thread(&IComponent::updateComponent,component),
          ILoggable ("ComponentThread"),
          mComponent(component)

    {
        auto log = getLog();
        log->trace(
            "Constructing Update Thread for {}", dynamic_cast<ILoggable*>(component.get())->getClassName()
        );
    }

    ComponentThread::~ComponentThread()
    {
       auto log=getLog();
       log->trace("Destructing");
    }
}
