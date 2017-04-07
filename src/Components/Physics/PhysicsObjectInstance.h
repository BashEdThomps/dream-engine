#ifndef PhysicsObjectInstance_h
#define PhysicsObjectInstance_h

#include "../../Constants.h"
#include "../../AssetInstance.h"
#include "PhysicsMotionState.h"
#include <btBulletDynamicsCommon.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Dream {
  class PhysicsObjectInstance : public AssetInstance {
  private:
      static map<string,const aiScene*> AssimpModelCache;
      static ::Assimp::Importer mImporter;
      static const aiScene* getModelFromCache(string);

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
      bool createCollisionShape(string projectPath);
      btCollisionShape* getCollisionShape();
      btRigidBody* getRigidBody();
      void getWorldTransform(btTransform&);
      void setInPhysicsWorld(bool);
      bool getInPhysicsWorld();
      btCollisionObject* getCollisionObject();
      void setLinearVelocity(float, float, float);
  protected:
      void loadExtraAttributes(nlohmann::json);

      void processAssimpNode(aiNode*,const aiScene*, btTriangleMesh* triMesh);
      void processAssimpMesh(aiMesh*, btTriangleMesh*);
  }; // End of PhysicsObjectInstance
} // End of Dream

#endif /* PhysicsObjectInstance_h */
