#pragma once

#include <memory>
#include <optional>

using std::optional;
using std::reference_wrapper;

namespace octronic::dream
{
    class SceneRuntime;
    class EntityRuntime;
    class ProjectRuntime;
    /**
     * @brief Base class for Component classes.
     */
    class Component
    {
    public:
        Component();
        Component(ProjectRuntime& pr);

        Component(Component&&) = default;
        Component& operator=(Component&&) = default;

        Component(const Component&) = delete;
        Component& operator=(const Component&) = delete;

        virtual bool init() = 0;
        virtual void pushTasks() = 0;
        void setProjectRuntime(ProjectRuntime& pr);
        ProjectRuntime& getProjectRuntime();
    protected:
         optional<reference_wrapper<ProjectRuntime>> mProjectRuntime;
    };
}
