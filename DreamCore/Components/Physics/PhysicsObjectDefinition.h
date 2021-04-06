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

#include "CompoundChildDefinition.h"

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
  class PhysicsObjectDefinition : public AssetDefinition
  {
  public:
    PhysicsObjectDefinition(ProjectDefinition& pd, const json& js);

    PhysicsObjectDefinition(PhysicsObjectDefinition&&) = default;
    PhysicsObjectDefinition& operator=(PhysicsObjectDefinition&&) = default;

    // Physics
    void  setMass(float mass);
    float getMass() const;

    void  setMargin(float margin);
    float getMargin() const;

    void setKinematic(bool kinematic);
    bool getKinematic() const;

    void setHalfExtents(const vec3& halfExtent);
    vec3 getHalfExtents() const;

    void  setNormal(vec3 normal);
    vec3 getNormal() const;

    float getRadius() const;
    void  setRadius(float rad);

    float getHeight() const;
    void setHeight(float height);

    float getConstant() const;
    void  setConstant(float constant);

    bool getControllableCharacter() const;
    void setControllableCharacter(bool controllable);

    void setCcdSweptSphereRadius(float rad);
    float getCcdSweptSphereRadius() const;

    void addCompoundChild(const CompoundChildDefinition& child);
    void removeCompoundChild(const CompoundChildDefinition& def);
    vector<CompoundChildDefinition> getCompoundChildren();
    void updateCompoundChildTransform(const CompoundChildDefinition& );

    float getRestitution() const;
    void setRestitution(float r);

    float getFriction() const;
    void  setFriction(float r);

    UuidType getCollisionModelUuid() const;
    void setCollisionModelUuid(UuidType);

    vec3 getLinearFactor() const;
    void setLinearFactor(const vec3& lf);

    vec3 getAngularFactor() const;
    void setAngularFactor(const vec3& af);

    vec3 getLinearVelocity() const;
    void setLinearVelocity(const vec3& lf);
    vec3 getAngularVelocity() const;
    void setAngularVelocity(const vec3& af);

  protected:
    void makeCompoundChildren();
  };
}
