#pragma once

#include <thread>
#include "../Common/DreamObject.h"

using std::thread;

namespace Dream
{
    class IComponent;
    class SceneRuntime;

    class ComponentThread : public thread, public DreamObject
    {
    public:
        ComponentThread(shared_ptr<IComponent> component);
        ~ComponentThread() override;
    protected:
        shared_ptr<IComponent> mComponent;
    };
}
