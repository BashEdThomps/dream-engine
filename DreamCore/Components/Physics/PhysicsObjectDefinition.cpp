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
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MASS] = mass;

        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getMass
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_MASS].is_number())
            {
                mJson[Constants::ASSET_ATTR_MASS] = 0;
            }
            return mJson[Constants::ASSET_ATTR_MASS];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setMargin
    (float margin)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_MARGIN] = margin;
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getMargin
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_MARGIN].is_number())
            {
                mJson[Constants::ASSET_ATTR_MARGIN] = 0;
            }
            return mJson[Constants::ASSET_ATTR_MARGIN];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setKinematic
    (bool kinematic)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_KINEMATIC] = kinematic;
        } dreamElseLockFailed
    }

    bool
    PhysicsObjectDefinition::getKinematic
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_KINEMATIC) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_KINEMATIC] = false;
            }
            return mJson[Constants::ASSET_ATTR_KINEMATIC];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::makeHalfExtentsObject
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_SIZE) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_SIZE] = json::object();
            }
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setHalfExtentsX
    (float halfExtentX)
    {
        if(dreamTryLock()) {
            dreamLock();
            makeHalfExtentsObject();
            mJson[Constants::ASSET_ATTR_SIZE][Constants::X] = halfExtentX;
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getHalfExtentsX
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            makeHalfExtentsObject();
            if (mJson[Constants::ASSET_ATTR_SIZE].find(Constants::X) == mJson[Constants::ASSET_ATTR_SIZE].end())
            {
                mJson[Constants::ASSET_ATTR_SIZE][Constants::X] = 0;
            }
            return mJson[Constants::ASSET_ATTR_SIZE][Constants::X];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setHalfExtentsY
    (float halfExtentY)
    {
        if(dreamTryLock()) {
            dreamLock();
            makeHalfExtentsObject();
            mJson[Constants::ASSET_ATTR_SIZE][Constants::Y] = halfExtentY;
        } dreamElseLockFailed

    }

    float
    PhysicsObjectDefinition::getHalfExtentsY
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            makeHalfExtentsObject();
            if (mJson[Constants::ASSET_ATTR_SIZE].find(Constants::Y) == mJson[Constants::ASSET_ATTR_SIZE].end())
            {
                mJson[Constants::ASSET_ATTR_SIZE][Constants::Y] = 0;
            }
            return mJson[Constants::ASSET_ATTR_SIZE][Constants::Y];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setHalfExtentsZ
    (float halfExtentZ)
    {
        if(dreamTryLock()) {
            dreamLock();
            makeHalfExtentsObject();
            mJson[Constants::ASSET_ATTR_SIZE][Constants::Z] = halfExtentZ;
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getHalfExtentsZ
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            makeHalfExtentsObject();
            if (mJson[Constants::ASSET_ATTR_SIZE].find(Constants::Z) == mJson[Constants::ASSET_ATTR_SIZE].end())
            {
                mJson[Constants::ASSET_ATTR_SIZE][Constants::Z] = 0;
            }
            return mJson[Constants::ASSET_ATTR_SIZE][Constants::Z];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::makeNormalObject
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_NORMAL) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_NORMAL] = json::object();
            }
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setNormalX
    (float halfExtentX)
    {
        if(dreamTryLock()) {
            dreamLock();
            makeNormalObject();
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::X] = halfExtentX;
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getNormalX
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            makeNormalObject();
            if (mJson[Constants::ASSET_ATTR_NORMAL].find(Constants::X) == mJson[Constants::ASSET_ATTR_NORMAL].end())
            {
                mJson[Constants::ASSET_ATTR_NORMAL][Constants::X] = 0;
            }
            return mJson[Constants::ASSET_ATTR_NORMAL][Constants::X];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setNormalY
    (float halfExtentY)
    {
        if(dreamTryLock()) {
            dreamLock();
            makeNormalObject();
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y] = halfExtentY;
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getNormalY
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            makeNormalObject();
            if (mJson[Constants::ASSET_ATTR_NORMAL].find(Constants::Y) == mJson[Constants::ASSET_ATTR_NORMAL].end())
            {
                mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y] = 0;
            }
            return mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setNormalZ
    (float halfExtentZ)
    {
        if(dreamTryLock()) {
            dreamLock();
            makeNormalObject();
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z] = halfExtentZ;
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getNormalZ
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            makeNormalObject();
            if (mJson[Constants::ASSET_ATTR_NORMAL].find(Constants::Z) == mJson[Constants::ASSET_ATTR_NORMAL].end())
            {
                mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z] = 0;
            }
            return mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z];
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getRadius
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_RADIUS) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_RADIUS] = 0;
            }
            return mJson[Constants::ASSET_ATTR_RADIUS];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setRadius
    (float rad)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_RADIUS] = rad;
        } dreamElseLockFailed
    }

    float PhysicsObjectDefinition::getHeight()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_HEIGHT) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_HEIGHT] = 0;
            }
            return mJson[Constants::ASSET_ATTR_HEIGHT];
        } dreamElseLockFailed

    }

    void PhysicsObjectDefinition::setHeight(float height)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_HEIGHT] = height;
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getConstant
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_CONSTANT) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_CONSTANT] = 0;
            }
            return mJson[Constants::ASSET_ATTR_CONSTANT];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setConstant
    (float constant)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_CONSTANT] = constant;
        } dreamElseLockFailed
    }

    bool
    PhysicsObjectDefinition::getControllableCharacter
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_CONTROLLABLE) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_CONTROLLABLE] = false;
            }
            return mJson[Constants::ASSET_ATTR_CONTROLLABLE];
        } dreamElseLockFailed

    }

    void
    PhysicsObjectDefinition::setControllableCharacter
    (bool controllable)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_CONTROLLABLE] = controllable;
        } dreamElseLockFailed
    }

    void PhysicsObjectDefinition::setCcdSweptSphereRadius(float rad)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_CCD_SPR] = rad;
        } dreamElseLockFailed
    }

    float PhysicsObjectDefinition::getCcdSweptSphereRadius()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::ASSET_ATTR_CCD_SPR) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_CCD_SPR] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_CCD_SPR];
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getRestitution
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_RESTITUTION) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_RESTITUTION] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_RESTITUTION];
        } dreamElseLockFailed

    }

    void
    PhysicsObjectDefinition::setRestitution
    (float r)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_RESTITUTION] = r;
        } dreamElseLockFailed
    }

    float
    PhysicsObjectDefinition::getFriction
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_FRICTION) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_FRICTION] = 0.0f;
            }
            return mJson[Constants::ASSET_ATTR_FRICTION];
        } dreamElseLockFailed

    }

    void
    PhysicsObjectDefinition::setFriction
    (float r)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_FRICTION] = r;
        } dreamElseLockFailed
    }


    void
    PhysicsObjectDefinition::addCompoundChild
    (CompoundChildDefinition def)
    {
        if(dreamTryLock()) {
            dreamLock();
            makeCompoundChildren();
            mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].push_back(def.getJson());
        } dreamElseLockFailed
    }

    vector<CompoundChildDefinition>
    PhysicsObjectDefinition::getCompoundChildren
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::updateCompoundChildTransform
    (CompoundChildDefinition def)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed

    }

    void
    PhysicsObjectDefinition::removeCompoundChild
    (CompoundChildDefinition def)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::makeCompoundChildren
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_COMPOUND_CHILDREN) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN] = json::array();
            }
        } dreamElseLockFailed
    }

    UuidType
    PhysicsObjectDefinition::getCollisionModel
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_COLLISION_MODEL].is_number())
            {
                mJson[Constants::ASSET_ATTR_COLLISION_MODEL] = 0;
            }

            return mJson[Constants::ASSET_ATTR_COLLISION_MODEL];
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setCollisionModel
    (UuidType modelUuid)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_COLLISION_MODEL] = modelUuid;
        } dreamElseLockFailed
    }

    Vector3
    PhysicsObjectDefinition::getLinearFactor
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_LINEAR_FENTITY].is_object())
            {
                mJson[Constants::ASSET_ATTR_LINEAR_FENTITY] = Vector3(0.0f).toJson();
            }
            return Vector3(mJson[Constants::ASSET_ATTR_LINEAR_FENTITY]);
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setLinearFactor
    (const Vector3& lf)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_LINEAR_FENTITY] = lf.toJson();
        } dreamElseLockFailed
    }

    Vector3
    PhysicsObjectDefinition::getAngularFactor
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY].is_object())
            {
                mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY] = Vector3(0.0f).toJson();
            }
            return Vector3(mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY]);
        } dreamElseLockFailed

    }

    void
    PhysicsObjectDefinition::setAngularFactor
    (const Vector3& af)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY] = af.toJson();
        } dreamElseLockFailed
    }

    Vector3
    PhysicsObjectDefinition::getLinearVelocity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY].is_object())
            {
                mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY] = Vector3(0.0f).toJson();
            }
            return Vector3(mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY]);
        } dreamElseLockFailed
    }

    void
    PhysicsObjectDefinition::setLinearVelocity
    (const Vector3& lf)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY] = lf.toJson();
        } dreamElseLockFailed
    }

    Vector3
    PhysicsObjectDefinition::getAngularVelocity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY].is_object())
            {
                mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY] = Vector3(0.0f).toJson();
            }
            return Vector3(mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY]);
        } dreamElseLockFailed

    }

    void
    PhysicsObjectDefinition::setAngularVelocity
    (const Vector3& af)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY] = af.toJson();
        } dreamElseLockFailed
    }
}
