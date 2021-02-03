/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#include "PhysicsObjectDefinition.h"

using std::unique_lock;

namespace octronic::dream
{
    PhysicsObjectDefinition::PhysicsObjectDefinition
    (ProjectDefinition* pd, const json &js)
        : AssetDefinition("PhysicsObjectDefinition",pd,js)
    {

    }

    void
    PhysicsObjectDefinition::setMass
    (float mass)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MASS] = mass;
    }

    float
    PhysicsObjectDefinition::getMass
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_MASS].is_number())
        {
            mJson[Constants::ASSET_ATTR_MASS] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MASS];
    }

    void
    PhysicsObjectDefinition::setMargin
    (float margin)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_MARGIN] = margin;
    }

    float
    PhysicsObjectDefinition::getMargin
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_MARGIN].is_number())
        {
            mJson[Constants::ASSET_ATTR_MARGIN] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MARGIN];
    }

    void
    PhysicsObjectDefinition::setKinematic
    (bool kinematic)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_KINEMATIC] = kinematic;
    }

    bool
    PhysicsObjectDefinition::getKinematic
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_KINEMATIC].is_null())
        {
            mJson[Constants::ASSET_ATTR_KINEMATIC] = false;
        }
        return mJson[Constants::ASSET_ATTR_KINEMATIC];
    }

    void
    PhysicsObjectDefinition::makeHalfExtentsObject
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_SIZE].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE] = json::object();
        }
    }

    void
    PhysicsObjectDefinition::setHalfExtentsX
    (float halfExtentX)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::X] = halfExtentX;
    }

    float
    PhysicsObjectDefinition::getHalfExtentsX
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeHalfExtentsObject();
        if (mJson[Constants::ASSET_ATTR_SIZE][Constants::X].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE][Constants::X] = 0;
        }
        return mJson[Constants::ASSET_ATTR_SIZE][Constants::X];
    }

    void
    PhysicsObjectDefinition::setHalfExtentsY
    (float halfExtentY)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::Y] = halfExtentY;

    }

    float
    PhysicsObjectDefinition::getHalfExtentsY
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeHalfExtentsObject();
        if (mJson[Constants::ASSET_ATTR_SIZE][Constants::Y].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE][Constants::Y] = 0;
        }
        return mJson[Constants::ASSET_ATTR_SIZE][Constants::Y];
    }

    void
    PhysicsObjectDefinition::setHalfExtentsZ
    (float halfExtentZ)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::Z] = halfExtentZ;
    }

    float
    PhysicsObjectDefinition::getHalfExtentsZ
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeHalfExtentsObject();
        if (mJson[Constants::ASSET_ATTR_SIZE][Constants::Z].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE][Constants::Z] = 0;
        }
        return mJson[Constants::ASSET_ATTR_SIZE][Constants::Z];
    }

    void
    PhysicsObjectDefinition::makeNormalObject
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_NORMAL].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL] = json::object();
        }
    }

    void
    PhysicsObjectDefinition::setNormalX
    (float halfExtentX)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::X] = halfExtentX;
    }

    float
    PhysicsObjectDefinition::getNormalX
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeNormalObject();
        if (mJson[Constants::ASSET_ATTR_NORMAL][Constants::X].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::X] = 0;
        }
        return mJson[Constants::ASSET_ATTR_NORMAL][Constants::X];
    }

    void
    PhysicsObjectDefinition::setNormalY
    (float halfExtentY)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y] = halfExtentY;
    }

    float
    PhysicsObjectDefinition::getNormalY
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeNormalObject();
        if (mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y] = 0;
        }
        return mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y];
    }

    void
    PhysicsObjectDefinition::setNormalZ
    (float halfExtentZ)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z] = halfExtentZ;
    }

    float
    PhysicsObjectDefinition::getNormalZ
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeNormalObject();
        if (mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z] = 0;
        }
        return mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z];
    }

    float
    PhysicsObjectDefinition::getRadius
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_RADIUS].is_null())
        {
            mJson[Constants::ASSET_ATTR_RADIUS] = 0;
        }
        return mJson[Constants::ASSET_ATTR_RADIUS];
    }

    void
    PhysicsObjectDefinition::setRadius
    (float rad)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_RADIUS] = rad;
    }

    float PhysicsObjectDefinition::getHeight()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_HEIGHT].is_null())
        {
            mJson[Constants::ASSET_ATTR_HEIGHT] = 0;
        }
        return mJson[Constants::ASSET_ATTR_HEIGHT];

    }

    void PhysicsObjectDefinition::setHeight(float height)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_HEIGHT] = height;
    }

    float
    PhysicsObjectDefinition::getConstant
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_CONSTANT].is_null())
        {
            mJson[Constants::ASSET_ATTR_CONSTANT] = 0;
        }
        return mJson[Constants::ASSET_ATTR_CONSTANT];
    }

    void
    PhysicsObjectDefinition::setConstant
    (float constant)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_CONSTANT] = constant;
    }

    bool
    PhysicsObjectDefinition::getControllableCharacter
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_CONTROLLABLE].is_null())
        {
            mJson[Constants::ASSET_ATTR_CONTROLLABLE] = false;
        }
        return mJson[Constants::ASSET_ATTR_CONTROLLABLE];

    }

    void
    PhysicsObjectDefinition::setControllableCharacter
    (bool controllable)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_CONTROLLABLE] = controllable;
    }

    void PhysicsObjectDefinition::setCcdSweptSphereRadius(float rad)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_CCD_SPR] = rad;
    }

    float PhysicsObjectDefinition::getCcdSweptSphereRadius()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::ASSET_ATTR_CCD_SPR].is_null())
        {
            mJson[Constants::ASSET_ATTR_CCD_SPR] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_CCD_SPR];
    }

    float
    PhysicsObjectDefinition::getRestitution
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_RESTITUTION].is_null())
        {
            mJson[Constants::ASSET_ATTR_RESTITUTION] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_RESTITUTION];

    }

    void
    PhysicsObjectDefinition::setRestitution
    (float r)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_RESTITUTION] = r;
    }

    float
    PhysicsObjectDefinition::getFriction
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_FRICTION].is_null())
        {
            mJson[Constants::ASSET_ATTR_FRICTION] = 0.0f;
        }
        return mJson[Constants::ASSET_ATTR_FRICTION];

    }

    void
    PhysicsObjectDefinition::setFriction
    (float r)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_FRICTION] = r;
    }


    void
    PhysicsObjectDefinition::addCompoundChild
    (CompoundChildDefinition def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       makeCompoundChildren();
       mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].push_back(def.getJson());
    }

    vector<CompoundChildDefinition>
    PhysicsObjectDefinition::getCompoundChildren
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeCompoundChildren();
        vector<CompoundChildDefinition> retval;
        for (json childJson : mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN])
        {
           CompoundChildDefinition def;
           def.parent = this;
           def.uuid = childJson[Constants::UUID];
           def.transform.fromJson(childJson[Constants::TRANSFORM]);
           retval.push_back(def);

        }
        return retval;
    }

    void
    PhysicsObjectDefinition::updateCompoundChildTransform
    (CompoundChildDefinition def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeCompoundChildren();
        for
        (
            auto iter = begin(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]);
            iter != end(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]);
            iter++
       )
       {
            UuidType jsUuid = (*iter)[Constants::UUID];
            if (def.uuid == jsUuid)
            {
               (*iter)[Constants::TRANSFORM] = def.transform.getJson();
                return;
            }
       }

    }

    void
    PhysicsObjectDefinition::removeCompoundChild
    (CompoundChildDefinition def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        makeCompoundChildren();
        for
        (
            auto iter = begin(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]);
            iter != end(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]);
            iter++
       )
       {
            if (*iter == def.getJson())
            {
               mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].erase(iter);
               return;
            }
        }
    }

    void
    PhysicsObjectDefinition::makeCompoundChildren
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].is_null())
        {
            mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN] = json::array();
        }
    }

    UuidType
    PhysicsObjectDefinition::getCollisionModel
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_COLLISION_MODEL].is_number())
        {
            mJson[Constants::ASSET_ATTR_COLLISION_MODEL] = 0;
        }

        return mJson[Constants::ASSET_ATTR_COLLISION_MODEL];
    }

    void
    PhysicsObjectDefinition::setCollisionModel
    (UuidType modelUuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_COLLISION_MODEL] = modelUuid;
    }

    Vector3
    PhysicsObjectDefinition::getLinearFactor
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_LINEAR_FENTITY].is_object())
        {
            mJson[Constants::ASSET_ATTR_LINEAR_FENTITY] = wrapVector3(Vector3(0.0f));
        }
        return unwrapVector3(mJson[Constants::ASSET_ATTR_LINEAR_FENTITY]);
    }

    void
    PhysicsObjectDefinition::setLinearFactor
    (const Vector3& lf)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_LINEAR_FENTITY] = wrapVector3(lf);
    }

    Vector3
    PhysicsObjectDefinition::getAngularFactor
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY].is_object())
        {
            mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY] = wrapVector3(Vector3(0.0f));
        }
        return unwrapVector3(mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY]);

    }

    void
    PhysicsObjectDefinition::setAngularFactor
    (const Vector3& af)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY] = wrapVector3(af);
    }

    Vector3
    PhysicsObjectDefinition::getLinearVelocity
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY].is_object())
        {
            mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY] = wrapVector3(Vector3(0.0f));
        }
        return unwrapVector3(mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY]);
    }

    void
    PhysicsObjectDefinition::setLinearVelocity
    (const Vector3& lf)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY] = wrapVector3(lf);
    }

    Vector3
    PhysicsObjectDefinition::getAngularVelocity
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY].is_object())
        {
            mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY] = wrapVector3(Vector3(0.0f));
        }
        return unwrapVector3(mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY]);

    }

    void
    PhysicsObjectDefinition::setAngularVelocity
    (const Vector3& af)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY] = wrapVector3(af);
    }
}
