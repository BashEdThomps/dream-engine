#ifndef PhysicsObjectInstance_h
#define PhysicsObjectInstance_h

#include "../../Constants.h"
#include "../../AssetInstance.h"
#include "PhysicsMotionState.h"
#include <btBulletDynamicsCommon.h>

namespace Dream {
  class PhysicsObjectInstance : public AssetInstance {
  private:
      btCollisionShape *mCollisionShape;
      btMotionState *mMotionState;
      btRigidBody *mRigidBody;
      btRigidBody::btRigidBodyConstructionInfo *mRigidBodyConstructionInfo;
      bool mInPhysicsWorld;
      bool mKinematic;
  public:
      PhysicsObjectInstance(AssetDefinition*,Transform3D*);
      ~PhysicsObjectInstance();
      bool load(string);
      bool createCollisionShape();
      btCollisionShape* getCollisionShape();
      btRigidBody* getRigidBody();
      void getWorldTransform(btTransform&);
      void setInPhysicsWorld(bool);
      bool getInPhysicsWorld();
      btCollisionObject* getCollisionObject();
  protected:
      void loadExtraAttributes(nlohmann::json);
  }; // End of PhysicsObjectInstance
} // End of Dream

#endif /* PhysicsObjectInstance_h */
