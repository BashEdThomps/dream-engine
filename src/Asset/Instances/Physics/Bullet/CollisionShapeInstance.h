#ifndef CollisionShapeInstance_h
#define CollisionShapeInstance_h

#include "../../../AssetInstance.h"
#include <btBulletDynamicsCommon.h>

#define COLLISION_SHAPE_SPHERE                   "btShpereShape"
#define COLLISION_SHAPE_BOX                      "btBoxShape"
#define COLLISION_SHAPE_CYLINDER                 "btCylinderShape"
#define COLLISION_SHAPE_CAPSULE                  "btCapsuleShape"
#define COLLISION_SHAPE_CONE                     "btConeShape"
#define COLLISION_SHAPE_MULTI_SPHERE             "btMultiSphereShape"
#define COLLISION_SHAPE_CONVEX_HULL              "btConvexHullShape"
#define COLLISION_SHAPE_CONVEX_TRIANGLE_MESH     "btConvexTriangleMeshShape"
#define COLLISION_SHAPE_CONVEX_BVH_TRIANGLE_MESH "btBvhTriangleMeshShape"
#define COLLISION_SHAPE_HEIGHTFIELD_TERRAIN      "btHeightfieldTerrainShape"
#define COLLISION_SHAPE_STATIC_PLANE             "btStaticPlaneShape"
#define COLLISION_SHAPE_COMPOUND                 "btCompoundShape"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Physics   {
namespace Bullet    {
	
	class CollisionShapeInstance : public Asset::AssetInstance {
	private:
		btCollisionShape *mCollisionShape;
	public:
		CollisionShapeInstance(AssetDefinition*);
		~CollisionShapeInstance();
		bool load(std::string);
		btCollisionShape* getCollisionShape();
	}; // End of CollisionShapeInstance
	
} // End of Bullet
} // End of Physics
} // End of Instances
} // End of Assets
} // End of Dream

#endif /* CollisionShapeInstance_h */
