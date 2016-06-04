#include "PhysicsObjectInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Physics   {
namespace Bullet    {
	
	PhysicsObjectInstance::PhysicsObjectInstance(AssetDefinition* definition) : AssetInstance(definition){}
	
	PhysicsObjectInstance::~PhysicsObjectInstance() {
		delete mRigidBody;
		delete mRigidBodyConstructionInfo;
		delete mMotionState;
		delete mCollisionShape;
	}
	
	btCollisionShape* PhysicsObjectInstance::getCollisionShape() {
		return mCollisionShape;
	}
		
	bool PhysicsObjectInstance::load(std::string projectPath) {
		if (!createCollisionShape()){
			std::cerr << "PhysicsObjectInstance: Unable to create collision shape" << std::endl;
			return false;
		}
		mMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		mRigidBodyConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo(0, mMotionState, mCollisionShape, btVector3(0, 0, 0));
		mRigidBody = new btRigidBody(*mRigidBodyConstructionInfo);
		return mRigidBody != NULL;
	}
	
	bool PhysicsObjectInstance::createCollisionShape() {
		std::string format = mDefinition->getFormat();
		//btScalar mass = mDefinition->getAttributeAsFloat(ASSET_ATTR_MASS);
		
		if (format.compare(COLLISION_SHAPE_SPHERE) == 0) {
			btScalar radius = mDefinition->getAttributeAsFloat(ASSET_ATTR_RADIUS);
			mCollisionShape = new btSphereShape(radius);
		} else if (format.compare(COLLISION_SHAPE_BOX) == 0) {
			//mCollisionShape = new btBoxShape();
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
		} else if (format.compare(COLLISION_SHAPE_CONVEX_BVH_TRIANGLE_MESH) == 0) {
			//mCollisionShape = new btBvhTriangleMeshShape();
		} else if (format.compare(COLLISION_SHAPE_HEIGHTFIELD_TERRAIN) == 0) {
			// ???
		} else if (format.compare(COLLISION_SHAPE_STATIC_PLANE) == 0) {
			//mCollisionShape = new btStaticPlaneShape();
		} else if (format.compare(COLLISION_SHAPE_COMPOUND) == 0) {
			//mCollisionShape = new btCompoundShape();
		}
		
		if (mCollisionShape) {
			btScalar margin = mDefinition->getAttributeAsFloat(ASSET_ATTR_MARGIN);
			mCollisionShape->setMargin(margin);
		}
		return mCollisionShape != NULL;
	}
	
	btRigidBody* PhysicsObjectInstance::getRigidBody() {
		return mRigidBody;
	}
	
	void PhysicsObjectInstance::getWorldTransform(btTransform &transform) {
		mMotionState->getWorldTransform(transform);
	}
	
} // End of Bullet
} // End of Physics
} // End of Instances
} // End of Assets
} // End of Dream
