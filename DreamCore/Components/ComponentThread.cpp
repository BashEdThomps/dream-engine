#include "ComponentThread.h"
#include "IComponent.h"
#include <spdlog/spdlog.h>

namespace Dream
{
    ComponentThread::ComponentThread(IComponent* component)
        : thread(&IComponent::updateComponent,component),
          ILoggable ("ComponentThread"),
          mComponentHandle(component)

    {
        auto log = getLog();
        log->trace(
            "Constructing Update Thread for {}", dynamic_cast<ILoggable*>(component)->getClassName()
        );
    }

    ComponentThread::~ComponentThread()
    {
       auto log=getLog();
       log->trace("Destructing");
    }
}
