#include <iostream>

#include "PhysicsRuntime.h"

#include "PhysicsMotionState.h"
#include "PhysicsComponent.h"
#include "PhysicsDefinition.h"
#include "PhysicsTasks.h"

#include "Common/Constants.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectDefinition.h"
#include "Project/ProjectRuntime.h"
#include "Components/Cache.h"

using std::make_shared;

namespace octronic::dream
{
  PhysicsRuntime::PhysicsRuntime
  (ProjectRuntime& prt,
   PhysicsDefinition& definition,
   EntityRuntime& entity)
    : DiscreteAssetRuntime(prt,definition,entity),
      mCollisionShape(nullptr),
      mMotionState(nullptr),
      mRigidBody(nullptr),
      mRigidBodyConstructionInfo(nullptr),
      mAddObjectTask(nullptr),
      mInPhysicsWorld(false)
  {
    LOG_TRACE( "PhysicsRuntime: Constructing" );
    mAddObjectTask = make_shared<PhysicsAddObjectTask>(getProjectRuntime(),*this);
  }

  btCollisionShape*
  PhysicsRuntime::getCollisionShape
  ()
  const
  {
    return mCollisionShape;
  }

  // This has a leak when reusing Physics Object Shapes
  bool
  PhysicsRuntime::loadFromDefinition
  ()
  {
    auto& pod = static_cast<PhysicsDefinition&>(getDefinition());
    mCollisionShape = createCollisionShape(pod);

    if (!mCollisionShape)
    {
      LOG_ERROR( "PhysicsRuntime: Unable to create collision shape" );
      return false;
    }

    float mass = pod.getMass();
    // Transform and CentreOfMass
    mMotionState = new PhysicsMotionState(getEntityRuntime());
    // Mass, MotionState, Shape and LocalInertia
    btVector3 inertia(0, 0, 0);
    mCollisionShape->calculateLocalInertia(mass, inertia);
    mRigidBodyConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo(
          btScalar(mass),mMotionState, mCollisionShape,inertia);

    mRigidBody = new btRigidBody(*mRigidBodyConstructionInfo);

    vec3 lf, lv, af, av;
    lf = pod.getLinearFactor();
    lv = pod.getLinearVelocity();
    af = pod.getAngularFactor();
    av = pod.getAngularVelocity();

    setLinearFactor(lf);
    setLinearVelocity(lv);
    setAngularFactor(af);
    setAngularVelocity(av);

    if (pod.getControllableCharacter())
    {
      setCameraControllableCharacter();
    }

    if (pod.getKinematic())
    {
      setKinematic(true);
    }

    setRestitution(pod.getRestitution());
    setFriction(pod.getFriction());
    setCcdSweptSphereRadius(pod.getCcdSweptSphereRadius());

    mLoaded = (mRigidBody != nullptr);


    return mLoaded;
  }

  void
  PhysicsRuntime::setCameraControllableCharacter
  ()
  {
    mRigidBody->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    mRigidBody->setActivationState(DISABLE_DEACTIVATION);
    mRigidBody->setAngularFactor(btVector3(0,1,0));
  }


  void
  PhysicsRuntime::setKinematic
  (bool setKenematic)
  {
    if (setKenematic)
    {
      mRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
      mRigidBody->setActivationState(DISABLE_DEACTIVATION);
    }
  }

  btCollisionShape*
  PhysicsRuntime::createCollisionShape
  (PhysicsDefinition& pod)
  {
    string format = pod.getFormat();
    btCollisionShape* collisionShape = nullptr;

    if (format == Constants::COLLISION_SHAPE_SPHERE)
    {
      btScalar radius = pod.getRadius();
      collisionShape = new btSphereShape(radius);
    }
    else if (format == Constants::COLLISION_SHAPE_BOX)
    {
      btVector3 box;
      box = Vector3::toBullet(pod.getHalfExtents());
      collisionShape = new btBoxShape(box);
    }
    else if (format == Constants::COLLISION_SHAPE_CYLINDER)
    {
      btVector3 box;
      box = Vector3::toBullet(pod.getHalfExtents());
      collisionShape = new btCylinderShape(box);
    }
    else if (format == Constants::COLLISION_SHAPE_CAPSULE)
    {
      float radius = pod.getRadius();
      float height = pod.getHeight();
      collisionShape = new btCapsuleShape(radius,height);
    }
    else if (format == Constants::COLLISION_SHAPE_CONE)
    {
      //collisionShape = new btConeShape();
    }
    else if (format == Constants::COLLISION_SHAPE_MULTI_SPHERE)
    {
      //collisionShape = new btMultiSphereShape();
    }
    else if (format == Constants::COLLISION_SHAPE_CONVEX_HULL)
    {
      //collisionShape = new btConvexHullShape();
    }
    else if (format == Constants::COLLISION_SHAPE_CONVEX_TRIANGLE_MESH)
    {
      //collisionShape = new btConvexTriangleMeshShape();
    }
    else if (format == Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH)
    {
      // Load Collision Data
      auto modelUuid = pod.getCollisionModelUuid();
      auto& pDef = static_cast<ProjectDefinition&>(getProjectRuntime().getDefinition());
      auto& modelDef = pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_MODEL,modelUuid).value().get();
      auto& modelCache = getProjectRuntime().getModelCache();
      auto& model = modelCache.getRuntime(static_cast<ModelDefinition&>(modelDef));
      collisionShape = createTriangleMeshShape(model);
    }
    else if (format == Constants::COLLISION_SHAPE_HEIGHTFIELD_TERRAIN)
    {
      // TODO
    }
    else if (format == Constants::COLLISION_SHAPE_STATIC_PLANE)
    {
      btVector3 planeNormal = Vector3::toBullet(pod.getNormal());
      float constant = pod.getConstant();

      auto planeConstant = btScalar(constant);

      collisionShape = new btStaticPlaneShape(planeNormal,planeConstant);
    }
    else if (format == Constants::COLLISION_SHAPE_COMPOUND)
    {
      /*
        collisionShape = make_unique<btCompoundShape>();
        btCompoundShape* compound = static_cast<btCompoundShape*>(collisionShape.get());

        for (CompoundChildDefinition& child : pod.getCompoundChildren())
        {
          auto def = getAssetDefinitionByUuid(child.uuid);
          auto shape = createCollisionShape(def);
          if (shape)
          {
            compound->addChildShape(child.transform.getBtTransform(),shape.get());
          }
        }
        */
    }

    if (collisionShape)
    {
      btScalar margin = pod.getMargin();
      collisionShape->setMargin(margin);
    }

    return collisionShape;
  }

  btCollisionShape*
  PhysicsRuntime::createTriangleMeshShape
  (ModelRuntime& model)
  {

    auto  meshes = model.getMeshes();

    if (meshes.empty())
    {
      throw std::exception();
    }

    btTriangleMesh* triMesh = new btTriangleMesh();
    for (auto& meshWrap : meshes)
    {
      auto& mesh = meshWrap.get();

      auto idx = mesh.getIndices();
      auto verts = mesh.getVertices();

      for (size_t i=0; i<idx.size(); i+=3)
      {
        btVector3 v1,v2,v3;

        auto i1 = verts.at(idx.at(i)).Position;
        auto i2 = verts.at(idx.at(i+1)).Position;
        auto i3 = verts.at(idx.at(i+2)).Position;

        v1.setX(i1.x);
        v1.setY(i1.y);
        v1.setZ(i1.z);

        v2.setX(i2.x);
        v2.setY(i2.y);
        v2.setZ(i2.z);

        v3.setX(i3.x);
        v3.setY(i3.y);
        v3.setZ(i3.z);

        triMesh->addTriangle(v1,v2,v3);
      }
      return new btBvhTriangleMeshShape(triMesh,true,true);
    }
    throw std::exception();
  }

  btRigidBody*
  PhysicsRuntime::getRigidBody
  ()
  const
  {
    return mRigidBody;
  }

  void
  PhysicsRuntime::getWorldTransform
  (btTransform &transform)
  {
    mMotionState->getWorldTransform(transform);
  }

  btCollisionObject*
  PhysicsRuntime::getCollisionObject
  ()
  const
  {
    return mRigidBody;
  }

  vec3
  PhysicsRuntime::getCenterOfMassPosition
  ()
  const
  {
    return Vector3::fromBullet(mRigidBody->getCenterOfMassPosition());
  }

  void
  PhysicsRuntime::applyCentralImpulse
  (const vec3& force)
  {
    mRigidBody->applyCentralImpulse(Vector3::toBullet(force));
  }

  void
  PhysicsRuntime::applyTorqueImpulse
  (const vec3& torque)
  {
    mRigidBody->applyTorqueImpulse(Vector3::toBullet(torque));
  }

  void
  PhysicsRuntime::applyForce
  (const vec3& force)
  {
    mRigidBody->applyForce(Vector3::toBullet(force),btVector3(0.0f,0.0f,0.0f));
  }

  void
  PhysicsRuntime::applyTorque
  (const vec3& torque)
  {
    mRigidBody->applyTorque(Vector3::toBullet(torque));
  }

  void
  PhysicsRuntime::clearForces
  ()
  {
    mRigidBody->clearForces();
  }

  void
  PhysicsRuntime::setCenterOfMassTransformTx
  (const Transform& tx)
  {
    mRigidBody->setCenterOfMassTransform(tx.getBtTransform());
  }

  void
  PhysicsRuntime::setCenterOfMassTransformMat4
  (const mat4& tx)
  {
    btTransform transform;
    transform.setFromOpenGLMatrix(value_ptr(tx));
    mRigidBody->setCenterOfMassTransform(transform);
  }

  void
  PhysicsRuntime::setCenterOfMassTransform3f
  (float x, float y, float z)
  {
    btTransform transform;
    auto current = mRigidBody->getCenterOfMassTransform();
    current.setOrigin(btVector3(x,y,z));
    mRigidBody->setCenterOfMassTransform(current);
  }


  void
  PhysicsRuntime::setCenterOfMassTransform3fv
  (const vec3& tx)
  {
    LOG_DEBUG("PhysicsRuntime: Setting Center of mass {},{},{}", tx.x, tx.y, tx.z);
    auto mtx = mRigidBody->getCenterOfMassTransform();
    mtx.setOrigin(Vector3::toBullet(tx));
    mRigidBody->setCenterOfMassTransform(mtx);
  }

  void
  PhysicsRuntime::setWorldTransform
  (const Transform& tx)
  {
    mRigidBody->setWorldTransform(tx.getBtTransform());
  }

  void
  PhysicsRuntime::setLinearVelocity
  (const vec3& lv)
  {
    mRigidBody->setLinearVelocity(Vector3::toBullet(lv));
  }

  vec3
  PhysicsRuntime::getLinearVelocity
  ()
  const
  {
    return Vector3::fromBullet(mRigidBody->getLinearVelocity());
  }

  void
  PhysicsRuntime::setLinearFactor
  (const vec3& lf)
  {
    mRigidBody->setLinearFactor(Vector3::toBullet(lf));
  }

  void
  PhysicsRuntime::setAngularFactor
  (const vec3& v)
  {
    mRigidBody->setAngularFactor(Vector3::toBullet(v));
  }

  void
  PhysicsRuntime::setAngularVelocity
  (const vec3& v)
  {
    mRigidBody->setAngularVelocity(Vector3::toBullet(v));
  }

  void
  PhysicsRuntime::setRestitution
  (float r)
  {
    mRigidBody->setRestitution(r);
  }

  float
  PhysicsRuntime::getFriction
  () const
  {
    return mRigidBody->getFriction();
  }

  void
  PhysicsRuntime::setFriction
  (float friction)
  {
    mRigidBody->setFriction(friction);
  }

  float
  PhysicsRuntime::getMass
  () const
  {
    return mRigidBody->getInvMass();
  }

  void
  PhysicsRuntime::setMass
  (float mass)
  {
    auto& pc = getProjectRuntime().getPhysicsComponent();
    pc.removeRigidBody(mRigidBody);
    btVector3 inertia(0.0f,0.0f,0.0f);
    mRigidBody->getCollisionShape()->calculateLocalInertia(mass,inertia);
    mRigidBody->setMassProps(mass,inertia);
    pc.addRigidBody(mRigidBody);
  }

  void
  PhysicsRuntime::setCcdSweptSphereRadius
  (float ccd)
  {
    if (ccd != 0.0f)
    {
      mRigidBody->setCcdMotionThreshold(1e-7f);
      mRigidBody->setCcdSweptSphereRadius(ccd);
    }
  }

  float
  PhysicsRuntime::getCcdSweptSphereRadius
  ()
  const
  {
    return mRigidBody->getCcdSweptSphereRadius();
  }

  float
  PhysicsRuntime::getRestitution
  ()
  const
  {
    return mRigidBody->getRestitution();
  }

  bool
  PhysicsRuntime::isInPhysicsWorld
  ()
  const
  {
    return mInPhysicsWorld;
  }

  void PhysicsRuntime::setInPhysicsWorld(bool inPhysicsWorld)
  {
    mInPhysicsWorld = inPhysicsWorld;
  }


  void PhysicsRuntime::pushTasks()
  {
    auto& taskQueue = getProjectRuntime().getTaskQueue();
    if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
    {
      taskQueue.pushTask(mLoadFromDefinitionTask);
    }
    else if (mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED) && !isInPhysicsWorld())
    {
      taskQueue.pushTask(mAddObjectTask);
    }
  }
}
