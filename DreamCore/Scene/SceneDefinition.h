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
#include "../Common/Definition.h"
#include "../Components/Transform.h"

using namespace std;

namespace Dream
{
    class ProjectDefinition;
    class SceneObject;
    class SceneObjectDefinition;

    class SceneDefinition : public Definition
    {

    private:
        vector<SceneObjectDefinition*> mTemplates;
        SceneObjectDefinition* mRootSceneObjectDefinition;
        ProjectDefinition* mProjectDefinition;

    public:
        SceneDefinition(ProjectDefinition* project, json data);
        ~SceneDefinition() override;

        void loadRootSceneObjectDefinition();

        void setCameraMovementSpeed(float speed);
        float getCameraMovementSpeed();

        void setPhysicsDebug(bool debug);
        bool getPhysicsDebug();

        void setMeshCullDistance(float mcd);
        float getMeshCullDistance();

        void addTemplate(SceneObjectDefinition* _template);
        SceneObjectDefinition* getTemplateByUuid(uint32_t uuid);

        void setMinDrawDistance(float mdd);
        float getMinDrawDistance();

        void setMaxDrawDistance(float mdd);
        float getMaxDrawDistance();

        vec3 getCameraTranslation();
        void setCameraTranslation(const vec3& trans);

        vec3 getCameraLookAt();
        void setCameraLookAt(const vec3& lookAt);

        vec3 getGravity();
        void setGravity(const vec3& gravity);
        void setGravityX(float);
        void setGravityY(float);
        void setGravityZ(float);

        vec3 getClearColour();
        void setClearColour(const vec3& clearColour);
        void setClearColourR(float);
        void setClearColourG(float);
        void setClearColourB(float);

        SceneObjectDefinition* getRootSceneObjectDefinition();
        ProjectDefinition* getProjectDefinition();
        SceneObjectDefinition* createNewRootSceneObjectDefinition();

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

        uint32_t getCameraFocusedOn();
        void setCameraFocusedOn(uint32_t focus);

        uint32_t getLightingPassShader();
        void setLightingPassShader(uint32_t shader);

        uint32_t getShadowPassShader();
        void setShadowPassShader(uint32_t shader);

        uint32_t getInputScript();
        void setInputScript(uint32_t shader);

        uint32_t getNanoVGScript();
        void setNanoVGScript(uint32_t shader);
    };
}
