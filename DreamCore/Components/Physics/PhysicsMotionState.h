#pragma once

#include <LinearMath/btMotionState.h>
#include <memory>

using std::shared_ptr;
using std::reference_wrapper;

class btTransform;

namespace octronic::dream
{
    class EntityRuntime;

    class PhysicsMotionState : public btMotionState
    {
    public:
        PhysicsMotionState(EntityRuntime& entity);
        ~PhysicsMotionState();

        void getWorldTransform(btTransform&) const override;
        void setWorldTransform(const btTransform&) override;

        void setKinematicPos(btTransform&);

        EntityRuntime& getEntityRuntime() const;
    protected:
        reference_wrapper<EntityRuntime> mEntityRuntime;
    };
}
