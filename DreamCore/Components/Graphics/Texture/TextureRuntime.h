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

#include "Common/GLHeader.h"

#include <iostream>
#include <memory>
#include "Components/SharedAssetRuntime.h"
#include "Components/Graphics/GraphicsComponentTasks.h"
#include "TextureTasks.h"

using std::vector;
using std::string;
using std::shared_ptr;

namespace octronic::dream
{
    class TextureDefinition;
    class EntityRuntime;
    class TextureRemoveFromGLTask;
    class TextureLoadIntoGLTask;
    class ShaderRuntime;

    class TextureRuntime : public SharedAssetRuntime
    {
    public:
        TextureRuntime(ProjectRuntime* pr, TextureDefinition* def);
        ~TextureRuntime() override;

        bool operator==(const TextureRuntime& other);

        bool loadFromDefinition() override;

        GLuint getTextureID() const;
        void setTextureID(const GLuint& id);

        GLuint getCubeMapTextureID() const;
        GLuint getIrradianceTextureID() const;
        GLuint getPreFilterTextureID() const;
        GLuint getBrdfLutTextureID() const;

        string getPath() const;
        void setPath(const string& path);

        int getWidth() const;
        void setWidth(int width);

        int getHeight() const;
        void setHeight(int height);

        int getChannels() const;
        void setChannels(int channels);

        void* getImage() const;
        void setImage(void* image);

        void pushTasks() override;

        bool loadTextureIntoGL();
        bool renderEquirectangularToCubeMap();
        bool renderIrradianceCubeMap();
        bool renderPreFilterCubeMap();
        bool renderBrdfLut();
        void renderCube();
        void renderQuad();

        bool getIsEnvironmentTexture() const;
        void setIsEnvironmentTexture(bool b);

        bool isHDR() const;

    private:
        bool mIsHDR;
        GLuint mGLTextureID;
        int mWidth;
        int mHeight;
        int mChannels;
        bool mIsEnvironmentTexture;
        void* mImage;
        GLuint mCaptureFBO;
        GLuint mCaptureRBO;
        // Equirectangular to Cube Map
        GLuint mEquiToCubeTexture;
        ShaderRuntime* mEquiToCubeShader;
        // Irradiance Map
        GLuint mIrradianceMapTexture;
        ShaderRuntime* mIrradianceMapShader;
        // Pre Filter
        GLuint mPreFilterCubeMapTexture;
        ShaderRuntime* mPreFilterShader;
        // BRDF LUT
        GLuint mBrdfLutTexture;
        ShaderRuntime* mBrdfLutShader;
        // Cube
        GLuint mCubeVAO;
        GLuint mCubeVBO;
        // Quad
        GLuint mQuadVAO;
    	GLuint mQuadVBO;
        // Tasks
        shared_ptr<TextureLoadIntoGLTask>    mLoadIntoGLTask;
        shared_ptr<TextureSetupEnvironmentTask> mRenderCubeMapTask;
        shared_ptr<TextureRemoveFromGLTask>  mRemoveFromGLTask;
		// Statics
        const static glm::mat4 CubeCaptureProjection;
        const static glm::mat4 CubeCaptureViews[6];
    };
}
