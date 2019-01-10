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

#pragma once

#include "../AssetDefinition.h"

namespace Dream
{

    class PhysicsObjectDefinition;

    class CompoundChildDefinition
    {
    public:
       PhysicsObjectDefinition* parent;
       Transform transform;
       uint32_t uuid;

       const json getJson()
       {
           json retval = json::object();
           retval[Constants::UUID] = uuid;
           retval[Constants::TRANSFORM] = transform.getJson();
           return retval;
       }
    };

    class PhysicsObjectDefinition
            : public AssetDefinition
    {
    public:
        PhysicsObjectDefinition(ProjectDefinition* pd, json js);

        // Physics
        void  setMass(float mass);
        float getMass();

        void  setMargin(float margin);
        float getMargin();

        void setKinematic(bool kinematic);
        bool getKinematic();

        void  setHalfExtentsX(float halfExtentX);
        float getHalfExtentsX();

        void  setHalfExtentsY(float halfExtentY);
        float getHalfExtentsY();

        void  setHalfExtentsZ(float halfExtentZ);
        float getHalfExtentsZ();

        void  setNormalX(float normalX);
        float getNormalX();

        void  setNormalY(float normalY);
        float getNormalY();

        void  setNormalZ(float normalZ);
        float getNormalZ();

        float getRadius();
        void  setRadius(float rad);

        float getHeight();
        void setHeight(float height);

        float getConstant();
        void  setConstant(float constant);

        bool getControllableCharacter();
        void setControllableCharacter(bool controllable);

        void setCcdSweptSphereRadius(float rad);
        float getCcdSweptSphereRadius();

        void addCompoundChild(CompoundChildDefinition child);
        void removeCompoundChild(CompoundChildDefinition def);
        vector<CompoundChildDefinition> getCompoundChildren();
        void updateCompoundChildTransform(CompoundChildDefinition);

        float getRestitution();
        void setRestitution(float r);

        float getFriction();
        void  setFriction(float r);

        uint32_t getCollisionModel();
        void setCollisionModel(uint32_t);

    protected:
        void makeCompoundChildren();
        void makeHalfExtentsObject();
        void makeNormalObject();
    };
}
