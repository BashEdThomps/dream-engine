#include "PhysicsObjectInstance.h"

namespace Dream {

    map<string,const aiScene*> PhysicsObjectInstance::AssimpModelCache = map<string,const aiScene*>();
    ::Assimp::Importer PhysicsObjectInstance::mImporter;

    const aiScene* PhysicsObjectInstance::getModelFromCache(string path) {
        map<string,const aiScene*>::iterator it;
        for (it=AssimpModelCache.begin();it!=AssimpModelCache.end();it++) {
            if ((*it).first.compare(path) == 0) {
                if (DEBUG) {
                    cout << "PhysicsObjectInstance: Found cached scene for " << path << endl;
                }
                return (*it).second;
            }
        }
        if (DEBUG) {
          cout << "PhysicsObjectInstance: Loading " << path << " from disk" << endl;
        }
        const aiScene* scene = mImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            cerr << "PhysicsObjectInstance: Error importing model " << mImporter.GetErrorString() << endl;
            return nullptr;
        }
        AssimpModelCache.insert(pair<string,const aiScene*>(path,scene));
        return scene;
    }

    PhysicsObjectInstance::PhysicsObjectInstance(AssetDefinition* definition,Transform3D* transform)
        : AssetInstance(definition,transform) {
        mInPhysicsWorld = false;
        mKinematic = false;
        return;
    }

    PhysicsObjectInstance::~PhysicsObjectInstance() {

        if (DEBUG) {
            cout << "PhysicsObjectInstance: Destroying Object" << endl;
        }

        if (mRigidBody != nullptr) {
            delete mRigidBody;
            mRigidBody = nullptr;
        }

        if (mRigidBodyConstructionInfo != nullptr) {
            delete mRigidBodyConstructionInfo;
            mRigidBodyConstructionInfo = nullptr;
        }

        if (mMotionState != nullptr) {
            delete mMotionState;
            mMotionState = nullptr;
        }

        if (mCollisionShape != nullptr) {
            delete mCollisionShape;
            mCollisionShape = nullptr;
        }
    }

    btCollisionShape* PhysicsObjectInstance::getCollisionShape() {
        return mCollisionShape;
    }

    void PhysicsObjectInstance::loadExtraAttributes(nlohmann::json jsonData) {
        // Margin
        if (!jsonData[ASSET_ATTR_MARGIN].is_null() && jsonData[ASSET_ATTR_MARGIN].is_number()) {
            mDefinition->addAttribute(ASSET_ATTR_MARGIN, to_string((static_cast<float>(jsonData[ASSET_ATTR_MARGIN]))));
        }
        // Mass
        if (!jsonData[ASSET_ATTR_MASS].is_null() && jsonData[ASSET_ATTR_MASS].is_number()) {
            mDefinition->addAttribute(ASSET_ATTR_MASS, to_string((static_cast<float>(jsonData[ASSET_ATTR_MASS]))));
        }
        // Radius
        if (!jsonData[ASSET_ATTR_RADIUS].is_null() && jsonData[ASSET_ATTR_RADIUS].is_number()) {
            mDefinition->addAttribute(ASSET_ATTR_RADIUS, to_string((static_cast<float>(jsonData[ASSET_ATTR_RADIUS]))));
        }
        // Normal
        if (!jsonData[ASSET_ATTR_NORMAL].is_null()) {
            nlohmann::json normal = jsonData[ASSET_ATTR_NORMAL];
            mDefinition->addAttribute(ASSET_ATTR_NORMAL_X, to_string((static_cast<float>(normal[ASSET_ATTR_X]))));
            mDefinition->addAttribute(ASSET_ATTR_NORMAL_Y, to_string((static_cast<float>(normal[ASSET_ATTR_Y]))));
            mDefinition->addAttribute(ASSET_ATTR_NORMAL_Z, to_string((static_cast<float>(normal[ASSET_ATTR_Z]))));
        }
        // Constant
        if (!jsonData[ASSET_ATTR_CONSTANT].is_null() && jsonData[ASSET_ATTR_CONSTANT].is_number()) {
            mDefinition->addAttribute(ASSET_ATTR_CONSTANT, to_string((static_cast<float>(jsonData[ASSET_ATTR_CONSTANT]))));
        }

        // Size
        if (!jsonData[ASSET_ATTR_SIZE].is_null()) {
            nlohmann::json size = jsonData[ASSET_ATTR_SIZE];
            mDefinition->addAttribute(ASSET_ATTR_SIZE_X, to_string((static_cast<float>(size[ASSET_ATTR_X]))));
            mDefinition->addAttribute(ASSET_ATTR_SIZE_Y, to_string((static_cast<float>(size[ASSET_ATTR_Y]))));
            mDefinition->addAttribute(ASSET_ATTR_SIZE_Z, to_string((static_cast<float>(size[ASSET_ATTR_Z]))));
        }
        // Kinematic
        if (!jsonData[ASSET_ATTR_KINEMATIC].is_null()) {
            mKinematic = static_cast<bool>(jsonData[ASSET_ATTR_KINEMATIC]);
        } else {
          mKinematic = false;
        }
    }

    bool PhysicsObjectInstance::load(string projectPath) {
        loadExtraAttributes(mDefinition->getJson());
        if (!createCollisionShape(projectPath)){
            cerr << "PhysicsObjectInstance: Unable to create collision shape" << endl;
            return false;
        }
        float mass = mDefinition->getAttributeAsFloat(ASSET_ATTR_MASS);
        // Transform and CentreOfMass
        mMotionState = new PhysicsMotionState(mTransform);
        // Mass, MotionState, Shape and LocalInertia
        btVector3 inertia(0, 0, 0);
        mCollisionShape->calculateLocalInertia(mass, inertia);
        mRigidBodyConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo(btScalar(mass), mMotionState, mCollisionShape, inertia);
        mRigidBody = new btRigidBody(*mRigidBodyConstructionInfo);
        if (mKinematic) {
          mRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
          mRigidBody->setActivationState(DISABLE_DEACTIVATION);
        }
        mLoaded = (mRigidBody != nullptr);
        return mLoaded;
    }

    bool PhysicsObjectInstance::createCollisionShape(string projectPath) {
        string format = mDefinition->getFormat();
        if (format.compare(COLLISION_SHAPE_SPHERE) == 0) {
            btScalar radius = mDefinition->getAttributeAsFloat(ASSET_ATTR_RADIUS);
            mCollisionShape = new btSphereShape(radius);
        } else if (format.compare(COLLISION_SHAPE_BOX) == 0) {
            btScalar boxX, boxY, boxZ;
            boxX = mDefinition->getAttributeAsFloat(ASSET_ATTR_SIZE_X);
            boxY = mDefinition->getAttributeAsFloat(ASSET_ATTR_SIZE_Y);
            boxZ = mDefinition->getAttributeAsFloat(ASSET_ATTR_SIZE_Z);
            mCollisionShape = new btBoxShape(btVector3(boxX,boxY,boxZ));
        } else if (format.compare(COLLISION_SHAPE_CYLINDER) == 0) {
            //mCollisionShape = new btCylinderShape();
        } else if (format.compare(COLLISION_SHAPE_CAPSULE) == 0) {
            //mCollisionShape = new btCapsuleShape();
        } else if (format.compare(COLLISION_SHAPE_CONE) == 0) {
            //mCollisionShape = new btConeShape();
        } else if (format.compare(COLLISION_SHAPE_MULTI_SPHERE) == 0) {
            //mCollisionShape = new btMultiSphereShape();
        } else if (format.compare(COLLISION_SHAPE_CONVEX_HULL) == 0) {
            //mCollisionShape = new btConvexHullShape();
        } else if (format.compare(COLLISION_SHAPE_CONVEX_TRIANGLE_MESH) == 0) {
            //mCollisionShape = new btConvexTriangleMeshShape();
        } else if (format.compare(COLLISION_SHAPE_BVH_TRIANGLE_MESH) == 0) {
            // Load Collision Data
            string path = projectPath+mDefinition->getAssetPath();
            if (DEBUG) {
                cout << "PhysicsObjectInstance: Loading collision geometry from "
                     << path << endl;
            }
            const aiScene* scene = getModelFromCache(path);
            btTriangleMesh *triMesh = new btTriangleMesh();
            processAssimpNode(scene->mRootNode, scene, triMesh);
            mCollisionShape = new btBvhTriangleMeshShape(triMesh,true,true);
        } else if (format.compare(COLLISION_SHAPE_HEIGHTFIELD_TERRAIN) == 0) {
            // ???
        } else if (format.compare(COLLISION_SHAPE_STATIC_PLANE) == 0) {
            float x = mDefinition->getAttributeAsFloat(ASSET_ATTR_NORMAL_X);
            float y = mDefinition->getAttributeAsFloat(ASSET_ATTR_NORMAL_Y);
            float z = mDefinition->getAttributeAsFloat(ASSET_ATTR_NORMAL_Z);
            float constant = mDefinition->getAttributeAsFloat(ASSET_ATTR_CONSTANT);
            btVector3 planeNormal(x,y,z);
            btScalar planeConstant = btScalar(constant);
            mCollisionShape = new btStaticPlaneShape(planeNormal,planeConstant);
        } else if (format.compare(COLLISION_SHAPE_COMPOUND) == 0) {
            //mCollisionShape = new btCompoundShape();
        }

        if (mCollisionShape) {
            btScalar margin = mDefinition->getAttributeAsFloat(ASSET_ATTR_MARGIN);
            mCollisionShape->setMargin(margin);
        }
        return mCollisionShape != nullptr;
    }

    void PhysicsObjectInstance::processAssimpNode(aiNode* node, const aiScene* scene, btTriangleMesh* triMesh) {
        // Process all the node's meshes (if any)
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            processAssimpMesh(mesh, triMesh);
        }
        // Then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processAssimpNode(node->mChildren[i],scene,triMesh);
        }
    }

    void PhysicsObjectInstance::processAssimpMesh(aiMesh* mesh, btTriangleMesh* triMesh) {
        for(unsigned int i = 0; i < mesh->mNumVertices; i+=3) {
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

    btRigidBody* PhysicsObjectInstance::getRigidBody() {
        return mRigidBody;
    }

    void PhysicsObjectInstance::getWorldTransform(btTransform &transform) {
        mMotionState->getWorldTransform(transform);
    }

    void PhysicsObjectInstance::setInPhysicsWorld(bool in) {
      mInPhysicsWorld = in;
    }

    bool PhysicsObjectInstance::getInPhysicsWorld() {
      return mInPhysicsWorld;
    }

    btCollisionObject* PhysicsObjectInstance::getCollisionObject() {
        return mRigidBody;
    }

    void PhysicsObjectInstance::setLinearVelocity(float x, float y, float z) {
        mRigidBody->setLinearVelocity(btVector3(x,y,z));
    }
} // End of Dream
