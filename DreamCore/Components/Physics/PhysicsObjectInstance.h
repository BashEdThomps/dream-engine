/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#include "../IAssetInstance.h"

#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <btBulletDynamicsCommon.h>

using std::string;
using std::map;
using std::vector;
using glm::vec3;

using nlohmann::json;

namespace Dream
{
    class PhysicsMotionState;
    class PhysicsObjectDefinition;
    class PhysicsComponent;

    class PhysicsObjectInstance : public IAssetInstance
    {
    private:
        btCollisionShape *mCollisionShape;
        btMotionState *mMotionState;
        btRigidBody *mRigidBody;
        btRigidBody::btRigidBodyConstructionInfo *mRigidBodyConstructionInfo;
        bool mInPhysicsWorld;
        PhysicsComponent* mPhysicsComponentHandle;
    public:
        PhysicsObjectInstance(
            PhysicsObjectDefinition*,
            PhysicsComponent*,
            SceneObjectRuntime*
        );
        ~PhysicsObjectInstance() override;
        bool load(string) override;
        btCollisionShape* createCollisionShape(PhysicsObjectDefinition*, string projectPath);
        btCollisionShape* getCollisionShape();
        btRigidBody* getRigidBody();
        void getWorldTransform(btTransform&);
        btCollisionObject* getCollisionObject();
        void setLinearVelocity(float, float, float);
        bool isInPhysicsWorld();
        void setInPhysicsWorld(bool inPhysicsWorld);
        void setLinearFactor(float x, float y, float z);
        void setAngularFactor(float x, float y, float z);
        vec3 getLinearVelocity();
        float getRestitution() const;
        void setRestitution(float r);
        float getFriction() const;
        void setFriction(float friction);
        float getMass() const;
        void setMass(float mass);
        void setAngularVelocity(float x, float y, float z);
    protected:
        PhysicsObjectDefinition* getAssetDefinitionByUuid(string);
    }; // End of PhysicsObjectInstance
} // End of Dream
