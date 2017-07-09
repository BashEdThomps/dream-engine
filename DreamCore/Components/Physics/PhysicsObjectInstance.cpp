#include "PhysicsObjectInstance.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PhysicsMotionState.h"

#include "../../Common/Constants.h"

#include "../../Project/ProjectDefinition.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

#include "../AssetDefinition.h"

namespace Dream
{

    map<string,const aiScene*> PhysicsObjectInstance::AssimpModelCache = map<string,const aiScene*>();
    ::Assimp::Importer PhysicsObjectInstance::mImporter;

    const aiScene* PhysicsObjectInstance::getModelFromCache(string path)
    {
        for (pair<string,const aiScene*> it : AssimpModelCache)
        {
            if (it.first.compare(path) == 0)
            {
                if (Constants::DEBUG)
                {
                    cout << "PhysicsObjectInstance: Found cached scene for " << path << endl;
                }
                return it.second;
            }
        }
        if (Constants::DEBUG)
        {
            cout << "PhysicsObjectInstance: Loading " << path << " from disk" << endl;
        }
        const aiScene* scene = mImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cerr << "PhysicsObjectInstance: Error importing model " << mImporter.GetErrorString() << endl;
            return nullptr;
        }
        AssimpModelCache.insert(pair<string,const aiScene*>(path,scene));
        return scene;
    }

    bool PhysicsObjectInstance::isInPhysicsWorld()
    {
        return mInPhysicsWorld;
    }

    void PhysicsObjectInstance::setInPhysicsWorld(bool inPhysicsWorld)
    {
        mInPhysicsWorld = inPhysicsWorld;
    }

    PhysicsObjectInstance::PhysicsObjectInstance
    (AssetDefinition* definition,SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          mInPhysicsWorld(false)
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsObjectInstance: Constructing" << endl;
        }
        return;
    }

    PhysicsObjectInstance::~PhysicsObjectInstance
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsObjectInstance: Destroying" << endl;
        }

        /***** Deletes are handled by PhysicsComponent! *****/

        if (mRigidBody != nullptr)
        {
            //delete mRigidBody;
            mRigidBody = nullptr;
        }

        if (mMotionState != nullptr)
        {
            //delete mMotionState;
            mMotionState = nullptr;
        }

        if (mCollisionShape != nullptr)
        {
            //delete mCollisionShape;
            mCollisionShape = nullptr;

        }

        if (mRigidBodyConstructionInfo != nullptr)
        {
            delete mRigidBodyConstructionInfo;
            mRigidBodyConstructionInfo = nullptr;
        }
    }

    btCollisionShape*
    PhysicsObjectInstance::getCollisionShape
    ()
    {
        return mCollisionShape;
    }

    void
    PhysicsObjectInstance::loadExtraAttributes
    (nlohmann::json jsonData)
    {
        loadExtraAttributes(jsonData,mDefinitionHandle,false);
    }

    void
    PhysicsObjectInstance::loadExtraAttributes
    (nlohmann::json jsonData, AssetDefinition* definition, bool isChild)
    {
    }

    bool
    PhysicsObjectInstance::load
    (string projectPath)
    {
        loadExtraAttributes(mDefinitionHandle->getJson(),mDefinitionHandle,false);
        mCollisionShape = createCollisionShape(mDefinitionHandle,projectPath);
        if (!mCollisionShape)
        {
            cerr << "PhysicsObjectInstance: Unable to create collision shape" << endl;
            return false;
        }
        float mass = mDefinitionHandle->getJson()[Constants::ASSET_ATTR_MASS];
        // Transform and CentreOfMass
        mMotionState = new PhysicsMotionState(mSceneObjectRuntimeHandle->getTransform());
        // Mass, MotionState, Shape and LocalInertia
        btVector3 inertia(0, 0, 0);
        mCollisionShape->calculateLocalInertia(mass, inertia);
        mRigidBodyConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo
        (
            btScalar(mass),
            mMotionState,
            mCollisionShape,
            inertia
        );

        mRigidBody = new btRigidBody(*mRigidBodyConstructionInfo);

        if (mDefinitionHandle->getPhysicsObjectKinematic())
        {
            mRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
            mRigidBody->setActivationState(DISABLE_DEACTIVATION);
        }

        mLoaded = (mRigidBody != nullptr);

        return mLoaded;
    }

    btCollisionShape*
    PhysicsObjectInstance::createCollisionShape
    (AssetDefinition* definition, string projectPath)
    {
        string format = definition->getFormat();
        btCollisionShape *collisionShape = NULL;

        if (format.compare(Constants::COLLISION_SHAPE_SPHERE) == 0)
        {
            btScalar radius = definition->getPhysicsObjectRadius();
            collisionShape = new btSphereShape(radius);
        }
        else if (format.compare(Constants::COLLISION_SHAPE_BOX) == 0)
        {
            btScalar boxX, boxY, boxZ;
            boxX = definition->getPhysicsObjectHalfExtentsX();
            boxY = definition->getPhysicsObjectHalfExtentsY();
            boxZ = definition->getPhysicsObjectHalfExtentsZ();
            collisionShape = new btBoxShape(btVector3(boxX,boxY,boxZ));
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CYLINDER) == 0)
        {
            //collisionShape = new btCylinderShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CAPSULE) == 0)
        {
            //collisionShape = new btCapsuleShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CONE) == 0)
        {
            //collisionShape = new btConeShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_MULTI_SPHERE) == 0)
        {
            //collisionShape = new btMultiSphereShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CONVEX_HULL) == 0)
        {
            //collisionShape = new btConvexHullShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CONVEX_TRIANGLE_MESH) == 0)
        {
            //collisionShape = new btConvexTriangleMeshShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH) == 0)
        {
            // Load Collision Data
            string path = projectPath+definition->getAssetPath();
            if (Constants::DEBUG)
            {
                cout << "PhysicsObjectInstance: Loading collision geometry from "
                     << path << endl;
            }
            const aiScene* scene = getModelFromCache(path);
            btTriangleMesh *triMesh = new btTriangleMesh();
            processAssimpNode(scene->mRootNode, scene, triMesh);
            collisionShape = new btBvhTriangleMeshShape(triMesh,true,true);
        }
        else if (format.compare(Constants::COLLISION_SHAPE_HEIGHTFIELD_TERRAIN) == 0)
        {
            // ???
        }
        else if (format.compare(Constants::COLLISION_SHAPE_STATIC_PLANE) == 0)
        {
            float x = definition->getPhysicsObjectNormalX();
            float y = definition->getPhysicsObjectNormalY();
            float z = definition->getPhysicsObjectNormalZ();
            float constant = definition->getPhysicsObjectConstant();

            btVector3 planeNormal(x,y,z);
            btScalar planeConstant = btScalar(constant);

            collisionShape = new btStaticPlaneShape(planeNormal,planeConstant);
        }
        else if (format.compare(Constants::COLLISION_SHAPE_COMPOUND) == 0)
        {
            collisionShape = new btCompoundShape();
            btCompoundShape *compound = static_cast<btCompoundShape*>(collisionShape);

            for (CompoundChildDefinition child : mDefinitionHandle->getPhysicsObjectCompoundChildren())
            {
                AssetDefinition  *def = getAssetDefinitionByUuid(child.uuid);
                loadExtraAttributes(def->getJson(),def,true);
                btCollisionShape *shape = createCollisionShape(def,projectPath);
                compound->addChildShape(child.transform.getTransformAsBtTransform(),shape);
            }
        }

        if (collisionShape)
        {
            btScalar margin = definition->getPhysicsObjectMargin();
            collisionShape->setMargin(margin);
        }

        return collisionShape;
    }

    void
    PhysicsObjectInstance::processAssimpNode
    (aiNode* node, const aiScene* scene, btTriangleMesh* triMesh)
    {
        // Process all the node's meshes (if any)

        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            processAssimpMesh(mesh, triMesh);
        }
        // Then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processAssimpNode(node->mChildren[i],scene,triMesh);
        }
    }

    void
    PhysicsObjectInstance::processAssimpMesh
    (aiMesh* mesh, btTriangleMesh* triMesh)
    {
        for(unsigned int i = 0; i < mesh->mNumVertices; i+=3)
        {
            btVector3 v1, v2, v3;
            // v1
            v1.setX(mesh->mVertices[i].x);
            v1.setY(mesh->mVertices[i].y);
            v1.setZ(mesh->mVertices[i].z);
            // v2
            v2.setX(mesh->mVertices[i+1].x);
            v2.setY(mesh->mVertices[i+1].y);
            v2.setZ(mesh->mVertices[i+1].z);
            // v3
            v3.setX(mesh->mVertices[i+2].x);
            v3.setY(mesh->mVertices[i+2].y);
            v3.setZ(mesh->mVertices[i+2].z);
            // Add triangle
            triMesh->addTriangle(v1,v2,v3);
        }
    }

    btRigidBody*
    PhysicsObjectInstance::getRigidBody
    ()
    {
        return mRigidBody;
    }

    void
    PhysicsObjectInstance::getWorldTransform
    (btTransform &transform)
    {
        mMotionState->getWorldTransform(transform);
    }

    btCollisionObject*
    PhysicsObjectInstance::getCollisionObject
    ()
    {
        return mRigidBody;
    }

    void
    PhysicsObjectInstance::setLinearVelocity
    (float x, float y, float z)
    {
        mRigidBody->setLinearVelocity(btVector3(x,y,z));
    }

    AssetDefinition*
    PhysicsObjectInstance::getAssetDefinitionByUuid
    (string uuid)
    {
        return mDefinitionHandle->getProjectHandle()->getAssetDefinitionHandleByUuid(uuid);
    }
} // End of Dream
