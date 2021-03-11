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

#include "Base/Definition.h"
#include "Math/Transform.h"
#include "Components/Graphics/CameraDefinition.h"

#include <vector>
#include <memory>

using glm::vec4;
using std::unique_ptr;

namespace octronic::dream
{
    class ProjectDefinition;
    class EntityDefinition;

    class SceneDefinition : public Definition
    {
    public:
        SceneDefinition(ProjectDefinition* project, const json& data);
        ~SceneDefinition() override;

        void loadRootEntityDefinition();

        // Rendering ===========================================================

        void setCamera(const json& cDef);
        CameraDefinition* getCamera();

        vec4 getClearColor();
        void setClearColor(const vec4& clear);

        UuidType getShadowPassShader();
        void setShadowPassShader(UuidType shader);

        UuidType getFontShader();
        void setFontShader(UuidType shader);

        UuidType getSpriteShader();
        void setSpriteShader(UuidType shader);

        UuidType getEnvironmentTexture();
        void setEnvironmentTexture(UuidType u);

        UuidType getEnvironmentShader();
        void setEnvironmentShader(UuidType u);

        // Input ===============================================================

        UuidType getInputScript();
        void setInputScript(UuidType shader);

        // Physics =============================================================

        vec3 getGravity();
        void setGravity(const vec3& gravity);

        // Entity Management ===================================================

        void addTemplate(EntityDefinition* _template);
        EntityDefinition* getTemplateByUuid(UuidType uuid);

        EntityDefinition* getRootEntityDefinition();
        ProjectDefinition* getProjectDefinition();
        EntityDefinition* createNewRootEntityDefinition();

        vector<string> getEntityNamesVector();

        // Serialisation =======================================================

        json toJson() override;

        // Variabels ===========================================================
    private:
        unique_ptr<CameraDefinition> mCameraDefinition;
        vector<EntityDefinition*> mTemplates;
        EntityDefinition* mRootEntityDefinition;
        ProjectDefinition* mProjectDefinition;
    };
}
