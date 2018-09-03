#pragma once

#include "../IAssetInstance.h"
#include "../../Common/ILoggable.h"

#include <string>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <btBulletDynamicsCommon.h>

using std::string;
using std::map;
using std::vector;

using nlohmann::json;

struct aiMesh;
struct aiNode;

namespace Dream
{
    class PhysicsMotionState;
    class PhysicsObjectDefinition;

    class PhysicsObjectInstance : public IAssetInstance, ILoggable
    {
    public:
        static void clearAssimpModelCache();

    private:

        static map<string,const aiScene*> AssimpModelCache;
        static ::Assimp::Importer mImporter;
        static const aiScene* getModelFromCache(string);
        btCollisionShape *mCollisionShape;
        btMotionState *mMotionState;
        btRigidBody *mRigidBody;
        btRigidBody::btRigidBodyConstructionInfo *mRigidBodyConstructionInfo;
        bool mInPhysicsWorld;

    public:
        PhysicsObjectInstance(shared_ptr<PhysicsObjectDefinition>, shared_ptr<SceneObjectRuntime>);
        ~PhysicsObjectInstance() override;
        bool load(string) override;
        btCollisionShape* createCollisionShape(shared_ptr<PhysicsObjectDefinition>, string projectPath);
        btCollisionShape* getCollisionShape();
        btRigidBody* getRigidBody();
        void getWorldTransform(btTransform&);
        btCollisionObject* getCollisionObject();
        void setLinearVelocity(float, float, float);

        bool isInPhysicsWorld();
        void setInPhysicsWorld(bool inPhysicsWorld);

    protected:
        void loadExtraAttributes(json) override;
        void loadExtraAttributes(json, shared_ptr<IAssetDefinition>, bool);
        void processAssimpNode(aiNode*,const aiScene*, btTriangleMesh* triMesh);
        void processAssimpMesh(aiMesh*, btTriangleMesh*);
        shared_ptr<PhysicsObjectDefinition> getAssetDefinitionByUuid(string);

    }; // End of PhysicsObjectInstance
} // End of Dream
