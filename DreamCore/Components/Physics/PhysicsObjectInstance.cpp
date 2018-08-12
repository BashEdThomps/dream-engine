#include "PhysicsObjectInstance.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PhysicsMotionState.h"

#include "../../Common/Constants.h"

#include "../../Project/ProjectDefinition.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

#include "PhysicsObjectDefinition.h"

namespace Dream
{

    map<string,const aiScene*> PhysicsObjectInstance::AssimpModelCache = map<string,const aiScene*>();
    ::Assimp::Importer PhysicsObjectInstance::mImporter;

    const aiScene* PhysicsObjectInstance::getModelFromCache(string path)
    {
        auto log = spdlog::stdout_color_mt("PhysicsObjectModelCache");
        for (pair<string,const aiScene*> it : AssimpModelCache)
        {
            if (it.first.compare(path) == 0)
            {
                log->info( "Found cached scene for {} ", path );
                return it.second;
            }
        }
        log->info( "Loading {} from disk", path);
        const aiScene* scene = mImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            log->error( "Error importing model {} " ,mImporter.GetErrorString() );
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
    (PhysicsObjectDefinition* definition,SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          ILoggable ("PhysicsObjectInstance"),
         mCollisionShape(nullptr),
         mMotionState(nullptr),
         mRigidBody(nullptr),
         mRigidBodyConstructionInfo(nullptr),
         mInPhysicsWorld(false) {
        auto log = getLog();
        log->trace( "Constructing" );
        return;
    }

    PhysicsObjectInstance::~PhysicsObjectInstance
    ()
    {
        auto log = getLog();
        log->trace( "Destroying" );

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
    (nlohmann::json jsonData, IAssetDefinition* definition, bool isChild)
    {
    }

    bool
    PhysicsObjectInstance::load
    (string projectPath)
    {
        auto log = getLog();
        PhysicsObjectDefinition* podHandle = dynamic_cast<PhysicsObjectDefinition*>
                (
                    mDefinitionHandle
                    );
        loadExtraAttributes(podHandle->getJson(),mDefinitionHandle,false);
        mCollisionShape = createCollisionShape(podHandle,projectPath);
        if (!mCollisionShape)
        {
            log->error( "Unable to create collision shape" );
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

        if (podHandle->getKinematic())
        {
            mRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
            mRigidBody->setActivationState(DISABLE_DEACTIVATION);
        }

        mLoaded = (mRigidBody != nullptr);

        return mLoaded;
    }

    btCollisionShape*
    PhysicsObjectInstance::createCollisionShape
    (PhysicsObjectDefinition* podHandle, string projectPath)
    {
        auto log = getLog();
        string format = podHandle->getFormat();
        btCollisionShape *collisionShape = nullptr;

        if (format.compare(Constants::COLLISION_SHAPE_SPHERE) == 0)
        {
            btScalar radius = podHandle->getRadius();
            collisionShape = new btSphereShape(radius);
        }
        else if (format.compare(Constants::COLLISION_SHAPE_BOX) == 0)
        {
            btScalar boxX, boxY, boxZ;
            boxX = podHandle->getHalfExtentsX();
            boxY = podHandle->getHalfExtentsY();
            boxZ = podHandle->getHalfExtentsZ();
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
            string path = projectPath+podHandle->getAssetPath();
            log->info( "Loading collision geometry from {}", path );
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
            float x = podHandle->getNormalX();
            float y = podHandle->getNormalY();
            float z = podHandle->getNormalZ();
            float constant = podHandle->getConstant();

            btVector3 planeNormal(x,y,z);
            btScalar planeConstant = btScalar(constant);

            collisionShape = new btStaticPlaneShape(planeNormal,planeConstant);
        }
        else if (format.compare(Constants::COLLISION_SHAPE_COMPOUND) == 0)
        {
            collisionShape = new btCompoundShape();
            btCompoundShape *compound = static_cast<btCompoundShape*>(collisionShape);

            for (CompoundChildDefinition child : podHandle->getCompoundChildren())
            {
                PhysicsObjectDefinition *def = getAssetDefinitionByUuid(child.uuid);
                loadExtraAttributes(def->getJson(),def,true);
                btCollisionShape *shape = createCollisionShape(def,projectPath);
                compound->addChildShape(child.transform.getTransformAsBtTransform(),shape);
            }
        }

        if (collisionShape)
        {
            btScalar margin = podHandle->getMargin();
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

    PhysicsObjectDefinition*
    PhysicsObjectInstance::getAssetDefinitionByUuid
    (string uuid)
    {
        return dynamic_cast<PhysicsObjectDefinition*>
                (
                    mDefinitionHandle
                    ->getProjectHandle()
                    ->getAssetDefinitionHandleByUuid(uuid)
                    );
    }
} // End of Dream
