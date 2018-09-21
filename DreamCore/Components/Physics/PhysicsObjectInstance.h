#pragma once

#include "../IAssetInstance.h"

#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <btBulletDynamicsCommon.h>

using std::string;
using std::map;
using std::vector;
using glm::vec3;

using nlohmann::json;

struct aiMesh;
struct aiNode;

namespace Dream
{
    class PhysicsMotionState;
    class PhysicsObjectDefinition;

    class PhysicsObjectInstance : public IAssetInstance
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
        PhysicsObjectInstance(
            PhysicsObjectDefinition*,
            SceneObjectRuntime*
        );
        ~PhysicsObjectInstance() override;
        bool load(string) override;
        btCollisionShape* createCollisionShape(PhysicsObjectDefinition*, string projectPath);
        btCollisionShape* getCollisionShape();
        btRigidBody* getRigidBody();
        void getWorldTransform(btTransform&);
        btCollisionObject* getCollisionObject();
        void setLinearVelocity(float, float, float);

        bool isInPhysicsWorld();
        void setInPhysicsWorld(bool inPhysicsWorld);

        void setLinearFactor(float x, float y, float z);
        void setAngularFactor(float x, float y, float z);
        vec3 getLinearVelocity();
        float getRestitution() const;
        void setRestitution(float r);
        float getFriction() const;
        void setFriction(float friction);

    protected:
        void loadExtraAttributes(json) override;
        void loadExtraAttributes(json, IAssetDefinition*, bool);
        void processAssimpNode(aiNode*,const aiScene*, btTriangleMesh* triMesh);
        void processAssimpMesh(aiMesh*, btTriangleMesh*);
        PhysicsObjectDefinition* getAssetDefinitionByUuid(string);

    }; // End of PhysicsObjectInstance
} // End of Dream
