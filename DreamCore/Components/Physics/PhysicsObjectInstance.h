#pragma once

#include <btBulletDynamicsCommon.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PhysicsMotionState.h"

#include "../../Common/Constants.h"
#include "../../Common/IAssetInstance.h"

namespace Dream
{

    struct CompoundChild
    {
        btTransform transform;
        string      uuid;
    };

    class PhysicsObjectInstance : public IAssetInstance
    {
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
        vector<CompoundChild> mCompoundChildren;
        vector<AssetDefinition*> mAssetDefinitions;
    public:
        PhysicsObjectInstance(AssetDefinition*,Transform3D*,vector<AssetDefinition*>);
        ~PhysicsObjectInstance();
        bool load(string);
        btCollisionShape* createCollisionShape(AssetDefinition*, string projectPath);
        btCollisionShape* getCollisionShape();
        btRigidBody* getRigidBody();
        void getWorldTransform(btTransform&);
        void setInPhysicsWorld(bool);
        bool getInPhysicsWorld();
        btCollisionObject* getCollisionObject();
        void setLinearVelocity(float, float, float);
    protected:
        void loadExtraAttributes(nlohmann::json);
        void loadExtraAttributes(nlohmann::json,AssetDefinition*,bool);
        void processAssimpNode(aiNode*,const aiScene*, btTriangleMesh* triMesh);
        void processAssimpMesh(aiMesh*, btTriangleMesh*);
        AssetDefinition* getAssetDefinitionByUuid(string);
    }; // End of PhysicsObjectInstance

} // End of Dream
