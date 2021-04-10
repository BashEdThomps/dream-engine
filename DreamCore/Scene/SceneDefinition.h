#pragma once

#include "Base/Definition.h"
#include "Math/Transform.h"
#include "Components/Graphics/CameraDefinition.h"
#include "Entity/SceneEntityDefinition.h"

#include <vector>
#include <memory>

using glm::vec4;

namespace octronic::dream
{
    class ProjectDefinition;

    class SceneDefinition : public Definition
    {
    public:
        SceneDefinition(ProjectDefinition& project, const json& data) ;

        SceneDefinition(SceneDefinition&&) = default;
        SceneDefinition& operator=(SceneDefinition&&) = default;

        // Rendering ===========================================================

        void setCameraDefinitionFromJson(const json& cDef);
        CameraDefinition& getCameraDefinition();

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

        // Scene Entity Management =============================================

        optional<SceneEntityDefinition>& getRootSceneEntityDefinition();
        optional<reference_wrapper<SceneEntityDefinition>> getSceneEntityByUuid(UuidType);

        // Project =============================================================

        ProjectDefinition& getProjectDefinition();

        // Serialisation =======================================================

        json getJson() override;

        // Variabels ===========================================================
    private:
        CameraDefinition mCameraDefinition;
        optional<SceneEntityDefinition> mRootSceneEntityDefinition;
        reference_wrapper<ProjectDefinition> mProjectDefinition;
    };
}
