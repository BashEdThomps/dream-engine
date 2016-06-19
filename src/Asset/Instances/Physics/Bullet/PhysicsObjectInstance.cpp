#include "PhysicsObjectInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Physics   {
namespace Bullet    {
	
	PhysicsObjectInstance::PhysicsObjectInstance(AssetDefinition* definition) : AssetInstance(definition) {}
	
	PhysicsObjectInstance::~PhysicsObjectInstance() {
		delete mRigidBody;
		delete mRigidBodyConstructionInfo;
		delete mMotionState;
		delete mCollisionShape;
	}
	
	btCollisionShape* PhysicsObjectInstance::getCollisionShape() {
		return mCollisionShape;
	}
	
	
	void PhysicsObjectInstance::loadExtraAttributes(nlohmann::json json) {
		// Margin
		if (!json[ASSET_ATTR_MARGIN].is_null() && json[ASSET_ATTR_MARGIN].is_number()) {
			mDefinition->addAttribute(ASSET_ATTR_MARGIN, std::to_string((float)json[ASSET_ATTR_MARGIN]));
		}
		// Mass
		if (!json[ASSET_ATTR_MASS].is_null() && json[ASSET_ATTR_MASS].is_number()) {
			mDefinition->addAttribute(ASSET_ATTR_MASS, std::to_string((float)json[ASSET_ATTR_MASS]));
		}
		// Radius
		if (!json[ASSET_ATTR_RADIUS].is_null() && json[ASSET_ATTR_RADIUS].is_number()) {
			mDefinition->addAttribute(ASSET_ATTR_RADIUS, std::to_string((float)json[ASSET_ATTR_RADIUS]));
		}
		// Normal
		if (!json[ASSET_ATTR_NORMAL].is_null()) {
			nlohmann::json normal = json[ASSET_ATTR_NORMAL];
			mDefinition->addAttribute(ASSET_ATTR_NORMAL_X, std::to_string((float)normal[ASSET_ATTR_X]));
			mDefinition->addAttribute(ASSET_ATTR_NORMAL_Y, std::to_string((float)normal[ASSET_ATTR_Y]));
			mDefinition->addAttribute(ASSET_ATTR_NORMAL_Z, std::to_string((float)normal[ASSET_ATTR_Z]));
		}
		// Constant
		if (!json[ASSET_ATTR_CONSTANT].is_null() && json[ASSET_ATTR_CONSTANT].is_number()) {
			mDefinition->addAttribute(ASSET_ATTR_CONSTANT, std::to_string((float)json[ASSET_ATTR_CONSTANT]));
		}
	}
		
	bool PhysicsObjectInstance::load(std::string projectPath) {
		if (!createCollisionShape()){
			std::cerr << "PhysicsObjectInstance: Unable to create collision shape" << std::endl;
			return false;
		}
		float mass = mDefinition->getAttributeAsFloat(ASSET_ATTR_MASS);
		// Transform and CentreOfMass
		mMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -3, 0)));
		// Mass, MotionState, Shape and LocalInertia
		btVector3 fallInertia(0, 0, 0);
		mCollisionShape->calculateLocalInertia(mass, fallInertia);
		mRigidBodyConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo(btScalar(mass), mMotionState, mCollisionShape, fallInertia);
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
