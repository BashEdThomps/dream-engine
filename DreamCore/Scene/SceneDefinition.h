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
#include "Math/Transform.h"

using glm::vec4;

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

        Transform getCameraTransform();
        void setCameraTransform(const Transform& trans);

        vec3 getGravity();
        void setGravity(const vec3& gravity);

        vec4 getClearColor();
        void setClearColor(const vec4& clear);

        EntityDefinition* getRootEntityDefinition();
        ProjectDefinition* getProjectDefinition();
        EntityDefinition* createNewRootEntityDefinition();

        json getJson() override;

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

        void setCameraFOV(float);
        float getCameraFOV();

        UuidType getEnvironmentTexture();
        void setEnvironmentTexture(UuidType u);

        UuidType getEnvironmentShader();
        void setEnvironmentShader(UuidType u);

    };
}
