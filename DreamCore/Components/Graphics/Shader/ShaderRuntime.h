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

#include "ShaderUniform.h"
#include "ShaderTasks.h"

#include "Common/GLHeader.h"
#include "Components/SharedAssetRuntime.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include <vector>
#include <map>
#include <memory>

using std::vector;
using std::map;
using std::shared_ptr;
using std::string;
using glm::mat4;
using glm::vec2;
using glm::vec4;

namespace octronic::dream
{
    class ShaderDefinition;
    class MaterialRuntime;
    class CameraRuntime;

    class ShaderRuntime : public SharedAssetRuntime
    {
    public: // Statics =========================================================
        const static size_t       MAX_RUNTIMES;

        const static GLint UNIFORM_NOT_FOUND;
        const static char* UNIFORM_VIEW_MATRIX;
        const static char* UNIFORM_MODEL_MATRIX;
        const static char* UNIFORM_PROJECTION_MATRIX;
        const static char* UNIFORM_CAMERA_POSITION;
        const static char* UNIFORM_COLOR;
        const static char* UNIFORM_SHADOW_TEXTURE;
        const static char* UNIFORM_SHADOW_SPACE_MATRIX;
        const static char* UNIFORM_MODEL_MATRIX_ARRAY;

        // PBR Textures
        const static char* UNIFORM_MATERIAL_ALBEDO;
        const static char* UNIFORM_MATERIAL_NORMAL;
        const static char* UNIFORM_MATERIAL_METALLIC;
        const static char* UNIFORM_MATERIAL_ROUGHNESS;
        const static char* UNIFORM_MATERIAL_AO;

        // PBR Environment
        const static char* UNIFORM_EQUIRECTANGULAR_MAP;
        const static char* UNIFORM_ENVIRONMENT_MAP;
        const static char* UNIFORM_ROUGHNESS_VALUE;
        const static char* UNIFORM_IRRADIANCE_TEXTURE;
        const static char* UNIFORM_PREFILTER_TEXTURE;
        const static char* UNIFORM_BRDF_LUT_TEXTURE;

        const static char* UNIFORM_LIGHT_POSITIONS;
        const static char* UNIFORM_LIGHT_COLORS;

        static map<GLenum,GLuint> CurrentTextures;
        static GLuint CurrentShaderProgram;
        static GLuint CurrentVAO;
        static GLuint CurrentVBO;
        static void InvalidateState();

    public: // Class ===========================================================
        ShaderRuntime(ProjectRuntime&,ShaderDefinition&);

        ShaderRuntime(ShaderRuntime&&) = default;
        ShaderRuntime& operator=(ShaderRuntime&&) = default;

        bool loadFromDefinition() override;
        bool init() override;
        void deleteUniforms();

        bool use();
        void unbind();

        GLuint getShaderProgram() const;
        void setShaderProgram(GLuint sp);
        void pushDestructionTask();

        // Uniform Setters =====================================================
        void setModelMatrixUniform(mat4);
        void setViewMatrixUniform(mat4);
        void setProjectionMatrixUniform(mat4);
        void setPositionUniform(vec2 pos);
        void setCameraPositionUniform(vec3);
        void setColorUniform(vec4 color);
        void setShadowTextureUniform(GLint shadow);
        void setShadowSpaceMatrixUniform(mat4 ssm);
        void setEquirectangularMapUniform(GLint map);
        void setEnvironmentMapUniform(GLint map);
        void setRoughnessUniform(float roughness);
        void setIrradianceTextureUniform(GLuint t);
        void setPreFilterTextureUniform(GLuint t);
        void setBrdfLutTextureUniform(GLuint t);
        void setLightPositionsUniform(vec3*, GLuint count);
        void setLightColorsUniform(vec3*, GLuint count);
        void setTexture(GLenum pos, GLenum target, GLuint texture);

        void syncUniforms();

    		bool checkUniformValue(ShaderUniform& uf);
        vector<ShaderUniform>& getUniformsVector();

        // Material ============================================================
        void bindMaterial(MaterialRuntime& material);
        void addMaterial(MaterialRuntime& material);
        void logMaterials() const;
        size_t countMaterials() const;
        vector<reference_wrapper<MaterialRuntime>> getMaterialsVector() const;

        void bindRuntimes(const vector<reference_wrapper<EntityRuntime>>& runtimes);

        // VAO =================================================================
        void bindVertexArray(GLuint);
        void unbindVertexArray();

        // Draw ================================================================
        void drawShadowPass(ShaderRuntime& shadowPassShader);

        // Sources =============================================================

        string getVertexSource() const;
        void setVertexSource(const string& vertexSource);

        string getFragmentSource() const;
        void setFragmentSource(const string& fragmentSource);

        void pushTasks() override;

        bool performFragmentCompilation();
        bool performVertexCompilation();
        bool performLinking();

        bool hasVertexSource() const;
        bool hasFragmentSource() const;
        bool hasVertexCompilationFailed() const;
        bool hasFragmentCompilationFailed() const;
        bool hasLinkingFailed() const;
        bool hasValidGLID() const;

    protected:
        GLint getUniformLocation(const string& name);

        bool readVertexSource();
        bool readFragmentSource();

        void addUniform(UniformType type, const string& name, int count, void* data);

        GLuint getVertexShader() const;
        void setVertexShader(const GLuint& vertexShader);

        GLuint getFragmentShader() const;
        void setFragmentShader(const GLuint& fragmentShader);

    private:
        GLuint mShaderProgram;

        bool mNeedsRebind;

        bool mVertexCompilationFailed;
        bool mFragmentCompilationFailed;
        bool mLinkingFailed;

        vector<ShaderUniform> mUniformVector;
        vector<reference_wrapper<MaterialRuntime>> mMaterials;
        vector<mat4> mRuntimeMatricies;
        map<string,GLint> mUniformLocationCache;
        shared_ptr<ShaderCompileFragmentTask> mCompileFragmentTask;
        shared_ptr<ShaderCompileVertexTask> mCompileVertexTask;
        shared_ptr<ShaderLinkTask> mLinkTask;
        shared_ptr<ShaderFreeTask> mFreeTask;
        string mVertexSource;
        GLuint mVertexShader;
        string mFragmentSource;
        GLuint mFragmentShader;
    };
}
