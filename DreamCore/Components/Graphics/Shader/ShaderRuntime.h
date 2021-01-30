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
#include "Components/Graphics/Light/LightRuntime.h"

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

namespace octronic::dream
{
    class ShaderCache;
    class ShaderDefinition;
    class MaterialRuntime;
    class LightRuntime;
    class Camera;

    class ShaderRuntime : public SharedAssetRuntime
    {
        public:
        const static unsigned int MAX_LIGHTS;
        const static GLint UNIFORM_NOT_FOUND;
        const static char* UNIFORM_POINT_LIGHT_COUNT;
        const static char* UNIFORM_SPOT_LIGHT_COUNT;
        const static char* UNIFORM_DIRECTIONAL_LIGHT_COUNT;
        const static size_t MAX_RUNTIMES;

        ShaderRuntime(ShaderDefinition*, ProjectRuntime*);
        ~ShaderRuntime() override;

        static GLuint CurrentTexture0;
        static GLuint CurrentTexture1;
        static GLuint CurrentTexture2;
        static GLuint CurrentTexture3;
        static GLuint CurrentTexture4;
        static GLuint CurrentTexture5;
        static GLuint CurrentShaderProgram;
        static GLuint CurrentVAO;
        static GLuint CurrentVBO;
        static void InvalidateState();

        bool useDefinition() override;
        void deleteUniforms();

        bool use();
        void unbind();
        GLuint getShaderProgram() const;
        void setShaderProgram(GLuint sp);
        int countMaterials();

        // MVP
        bool setModelMatrix(const mat4&,const string& name = "model");
        bool setViewMatrix(const mat4&,const string& name = "view");
        bool setProjectionMatrix(const mat4&,const string& name = "projection");

        // Viewer
        bool setViewerPosition(const Vector3&, const string& name = "viewPos");

        void bindVertexArray(GLuint);
        void unbindVertexArray();

        GLint getUniformLocation(const string& name);

        void addUniform(UniformType type, const string& name, int count, void* data);
        void bindLight(LightRuntime* light);

        void syncUniforms();

        void bindMaterial(MaterialRuntime* material);
        void bindLightQueue(const vector<LightRuntime*>& lightQueue);
        void bindRuntimes(const vector<EntityRuntime*>& runtimes);

        void addMaterial(MaterialRuntime* material);

        void logMaterials();

        void drawGeometryPass(Camera*);
        void drawShadowPass(ShaderRuntime* shadowPassShader);

        bool getRecompile() const;
        void setRecompile(bool recompile);

        string getVertexSource() const;
        void setVertexSource(const string& vertexSource);

        string getFragmentSource() const;
        void setFragmentSource(const string& fragmentSource);

        GLuint getVertexShader() const;
        void setVertexShader(const GLuint& vertexShader);

        GLuint getFragmentShader() const;
        void setFragmentShader(const GLuint& fragmentShader);

        GLint getPointLightCountLocation() const;
        void setPointLightCountLocation(const GLint& pointLightCountLocation);

        GLint getSpotLightCountLocation() const;
        void setSpotLightCountLocation(const GLint& spotLightCountLocation);

        GLint getDirectionalLightCountLocation() const;
        void setDirectionalLightCountLocation(const GLint& directionalLightCountLocation);

        void setFontColorUniform(const Vector3& color);
        void setFontPositionUniform(const vec2& pos);
        void setFontProjection(const mat4& proj);


		bool performFragmentCompilation();
		bool performVertexCompilation();
		bool performLinking();

    protected:
        bool compileVertex();
        bool compileFragment();
        bool linkProgram();

        ShaderCompileFragmentTask mCompileFragmentTask;
        ShaderCompileVertexTask mCompileVertexTask;
        ShaderLinkTask mLinkTask;
        shared_ptr<ShaderFreeTask> mFreeTask;
    private:
        unsigned int mPointLightCount;
        GLint mPointLightCountLocation;

        unsigned int mSpotLightCount;
        GLint mSpotLightCountLocation;

        unsigned int mDirectionalLightCount;
        GLint mDirectionalLightCountLocation;

        GLuint mShaderProgram;

        bool mNeedsRebind;
        GLint mMaterialLocation;

        vector<ShaderUniform*> mUniformVector;
        vector<MaterialRuntime*> mMaterials;
        vector<mat4> mRuntimeMatricies;
        map<string,GLint> mUinformCache;
        GLuint mVertexShader;
        GLuint mFragmentShader;
        bool mRecompile;
        string mVertexSource;
        string mFragmentSource;


    };
}
