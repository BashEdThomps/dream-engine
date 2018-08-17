/*
 * PhysicsObjectDefinition.cpp
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

namespace Dream
{
    PhysicsObjectDefinition::PhysicsObjectDefinition
    (ProjectDefinition* pdHandle, json js)
        : IAssetDefinition(pdHandle,js)
    {

    }

    void
    PhysicsObjectDefinition::setMass
    (float mass)
    {
        mJson[Constants::ASSET_ATTR_MASS] = mass;
    }

    float
    PhysicsObjectDefinition::getMass
    ()
    {
        if (mJson[Constants::ASSET_ATTR_MASS].is_null())
        {
            mJson[Constants::ASSET_ATTR_MASS] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MASS];
    }

    void
    PhysicsObjectDefinition::setMargin
    (float margin)
    {
        mJson[Constants::ASSET_ATTR_MARGIN] = margin;
    }

    float
    PhysicsObjectDefinition::getMargin
    ()
    {
        if (mJson[Constants::ASSET_ATTR_MARGIN].is_null())
        {
            mJson[Constants::ASSET_ATTR_MARGIN] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MARGIN];
    }

    void
    PhysicsObjectDefinition::setKinematic
    (bool kinematic)
    {
        mJson[Constants::ASSET_ATTR_KINEMATIC] = kinematic;
    }

    bool
    PhysicsObjectDefinition::getKinematic
    ()
    {
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
        if (mJson[Constants::ASSET_ATTR_SIZE].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE] = json::object();
        }
    }

    void
    PhysicsObjectDefinition::setHalfExtentsX
    (float halfExtentX)
    {
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::X] = halfExtentX;
    }

    float
    PhysicsObjectDefinition::getHalfExtentsX
    ()
    {
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
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::Y] = halfExtentY;

    }

    float
    PhysicsObjectDefinition::getHalfExtentsY
    ()
    {
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
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::Z] = halfExtentZ;
    }

    float
    PhysicsObjectDefinition::getHalfExtentsZ
    ()
    {
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
        if (mJson[Constants::ASSET_ATTR_NORMAL].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL] = json::object();
        }
    }

    void
    PhysicsObjectDefinition::setNormalX
    (float halfExtentX)
    {
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::X] = halfExtentX;
    }

    float
    PhysicsObjectDefinition::getNormalX
    ()
    {
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
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y] = halfExtentY;

    }

    float
    PhysicsObjectDefinition::getNormalY
    ()
    {
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
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z] = halfExtentZ;
    }

    float
    PhysicsObjectDefinition::getNormalZ
    ()
    {
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
        mJson[Constants::ASSET_ATTR_RADIUS] = rad;
    }

    float PhysicsObjectDefinition::getHeight()
    {
        if (mJson[Constants::ASSET_ATTR_HEIGHT].is_null())
        {
            mJson[Constants::ASSET_ATTR_HEIGHT] = 0;
        }
        return mJson[Constants::ASSET_ATTR_HEIGHT];

    }

    void PhysicsObjectDefinition::setHeight(float height)
    {
        mJson[Constants::ASSET_ATTR_HEIGHT] = height;
    }

    float
    PhysicsObjectDefinition::getConstant
    ()
    {
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
        mJson[Constants::ASSET_ATTR_CONSTANT] = constant;
    }

    bool
    PhysicsObjectDefinition::getControllableCharacter
    ()
    {
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
        mJson[Constants::ASSET_ATTR_CONTROLLABLE] = controllable;
    }

    void
    PhysicsObjectDefinition::addCompoundChild
    (CompoundChildDefinition def)
    {
       makeCompoundChildren();
       mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].push_back(def.getJson());
    }

    vector<CompoundChildDefinition>
    PhysicsObjectDefinition::getCompoundChildren
    ()
    {
        makeCompoundChildren();
        vector<CompoundChildDefinition> retval;
        for (json childJson : mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN])
        {
           CompoundChildDefinition def;
           def.uuid = childJson[Constants::UUID];
           def.transform = Transform3D(childJson[Constants::TRANSFORM]);
           retval.push_back(def);

        }
        return retval;
    }

    void
    PhysicsObjectDefinition::removeCompoundChild
    (CompoundChildDefinition def)
    {
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
        if (mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].is_null())
        {
            mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN] = json::array();
        }
    }

}
