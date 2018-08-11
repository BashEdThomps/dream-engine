#pragma once

#include <thread>
#include "../Common/ILoggable.h"

using std::thread;

namespace Dream
{
    class IComponent;
    class SceneRuntime;

    class ComponentThread : public thread, ILoggable
    {
    public:
        ComponentThread(IComponent* component);
        ~ComponentThread() override;
    protected:
        IComponent* mComponentHandle;
    };
}
