#ifndef PhysicsObjectInstance_h
#define PhysicsObjectInstance_h

#include "../../AssetInstance.h"
#include <btBulletDynamicsCommon.h>

// Collision Shape Attributes
#define ASSET_ATTR_MASS "mass"
#define ASSET_ATTR_MARGIN "margin"
#define ASSET_ATTR_RADIUS "radius"
#define ASSET_ATTR_NORMAL "normal"
#define ASSET_ATTR_CONSTANT "constant"

#define ASSET_ATTR_NORMAL_X "normalX"
#define ASSET_ATTR_NORMAL_Y "normalY"
#define ASSET_ATTR_NORMAL_Z "normalZ"

#define ASSET_ATTR_SIZE "size"

#define ASSET_ATTR_SIZE_X "sizeX"
#define ASSET_ATTR_SIZE_Y "sizeY"
#define ASSET_ATTR_SIZE_Z "sizeZ"

#define COLLISION_SHAPE_SPHERE "btShpereShape"
#define COLLISION_SHAPE_BOX "btBoxShape"
#define COLLISION_SHAPE_CYLINDER "btCylinderShape"
#define COLLISION_SHAPE_CAPSULE "btCapsuleShape"
#define COLLISION_SHAPE_CONE "btConeShape"
#define COLLISION_SHAPE_MULTI_SPHERE "btMultiSphereShape"
#define COLLISION_SHAPE_CONVEX_HULL "btConvexHullShape"
#define COLLISION_SHAPE_CONVEX_TRIANGLE_MESH "btConvexTriangleMeshShape"
#define COLLISION_SHAPE_CONVEX_BVH_TRIANGLE_MESH "btBvhTriangleMeshShape"
#define COLLISION_SHAPE_HEIGHTFIELD_TERRAIN "btHeightfieldTerrainShape"
#define COLLISION_SHAPE_STATIC_PLANE "btStaticPlaneShape"
#define COLLISION_SHAPE_COMPOUND "btCompoundShape"

namespace Dream {
    class PhysicsObjectInstance : public AssetInstance {
    private:
        btCollisionShape *mCollisionShape;
        btDefaultMotionState *mMotionState;
        btRigidBody *mRigidBody;
        btRigidBody::btRigidBodyConstructionInfo *mRigidBodyConstructionInfo;
    public:
        PhysicsObjectInstance(AssetDefinition*,Transform3D*);
        ~PhysicsObjectInstance();
        bool load(string);
        bool createCollisionShape();
        btCollisionShape* getCollisionShape();
        btRigidBody* getRigidBody();
        void getWorldTransform(btTransform&);
    protected:
        void loadExtraAttributes(nlohmann::json);
    }; // End of PhysicsObjectInstance
} // End of Dream

#endif /* PhysicsObjectInstance_h */
