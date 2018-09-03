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
        ComponentThread(shared_ptr<IComponent> component);
        ~ComponentThread() override;
    protected:
        shared_ptr<IComponent> mComponent;
    };
}
