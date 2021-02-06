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

#include <vector>
#include "Base/Definition.h"
#include "Components/Transform.h"


namespace octronic::dream
{
    class ProjectDefinition;
    class Entity;
    class EntityDefinition;

    class SceneDefinition : public Definition
    {

    private:
        vector<EntityDefinition*> mTemplates;
        EntityDefinition* mRootEntityDefinition;
        ProjectDefinition* mProjectDefinition;

    public:
        SceneDefinition(ProjectDefinition* project, const json& data);
        ~SceneDefinition() override;

        void loadRootEntityDefinition();

        void setCameraMovementSpeed(float speed);
        float getCameraMovementSpeed();

        void setPhysicsDebug(bool debug);
        bool getPhysicsDebug();

        void setMeshCullDistance(float mcd);
        float getMeshCullDistance();

        void addTemplate(EntityDefinition* _template);
        EntityDefinition* getTemplateByUuid(UuidType uuid);

        void setMinDrawDistance(float mdd);
        float getMinDrawDistance();

        void setMaxDrawDistance(float mdd);
        float getMaxDrawDistance();

        Vector3 getCameraTranslation();
        void setCameraTranslation(const Vector3& trans);

        Vector3 getCameraLookAt();
        void setCameraLookAt(const Vector3& lookAt);

        Vector3 getGravity();
        void setGravity(const Vector3& gravity);
        void setGravityX(float);
        void setGravityY(float);
        void setGravityZ(float);

        Vector3 getClearColour();
        void setClearColour(const Vector3& clearColour);
        void setClearColourR(float);
        void setClearColourG(float);
        void setClearColourB(float);

        EntityDefinition* getRootEntityDefinition();
        ProjectDefinition* getProjectDefinition();
        EntityDefinition* createNewRootEntityDefinition();

        json getJson() override;

        float getCameraTranslationX();
        float getCameraTranslationY();
        float getCameraTranslationZ();

        void setCameraTranslationX(float val);
        void setCameraTranslationY(float val);
        void setCameraTranslationZ(float val);

        void setCameraPitch(float pitch);
        float getCameraPitch();

        void setCameraYaw(float yaw);
        float getCameraYaw();

        UuidType getCameraFocusedOn();
        void setCameraFocusedOn(UuidType focus);

        UuidType getLightingPassShader();
        void setLightingPassShader(UuidType shader);

        UuidType getShadowPassShader();
        void setShadowPassShader(UuidType shader);

        UuidType getFontShader();
        void setFontShader(UuidType shader);

        UuidType getSpriteShader();
        void setSpriteShader(UuidType shader);

        UuidType getInputScript();
        void setInputScript(UuidType shader);

        void setPlayerObject(UuidType po);
        UuidType getPlayerObject();
    };
}
