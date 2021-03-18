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
using std::shared_ptr;

namespace octronic::dream
{
    class ProjectDefinition;
    class EntityDefinition;

    class SceneDefinition : public Definition
    {
    public:
        SceneDefinition(const weak_ptr<ProjectDefinition>& project, const json& data);
        ~SceneDefinition() override;

        void loadRootEntityDefinition();

        // Rendering ===========================================================

        void setCamera(const json& cDef);
        weak_ptr<CameraDefinition> getCamera() const;

        vec4 getClearColor() const;
        void setClearColor(const vec4& clear);

        UuidType getShadowPassShader() const;
        void setShadowPassShader(UuidType shader);

        UuidType getFontShader() const;
        void setFontShader(UuidType shader);

        UuidType getSpriteShader() const;
        void setSpriteShader(UuidType shader);

        UuidType getEnvironmentTexture() const;
        void setEnvironmentTexture(UuidType u);

        UuidType getEnvironmentShader() const;
        void setEnvironmentShader(UuidType u);

        // Input ===============================================================

        UuidType getInputScript() const;
        void setInputScript(UuidType shader);

        // Physics =============================================================

        vec3 getGravity() const;
        void setGravity(const vec3& gravity);

        // Entity Management ===================================================

        void addTemplate(const shared_ptr<EntityDefinition>& _template);
        weak_ptr<EntityDefinition> getTemplateByUuid(UuidType uuid) const;

        weak_ptr<EntityDefinition> getRootEntityDefinition() const;
        weak_ptr<ProjectDefinition> getProjectDefinition() const;
        void createNewRootEntityDefinition();

        vector<string> getEntityNamesVector() const;

        // Serialisation =======================================================

        json toJson() override;

        // Variabels ===========================================================
    private:
        shared_ptr<CameraDefinition> mCameraDefinition;
        vector<shared_ptr<EntityDefinition>> mTemplates;
        shared_ptr<EntityDefinition> mRootEntityDefinition;
        weak_ptr<ProjectDefinition> mProjectDefinition;
    };
}
